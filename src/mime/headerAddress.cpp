#include "stdafx.h"
#include "mime/headerAddress.hpp"
#include "utils/string.hpp"
#include "mime/utils.hpp"

namespace ccms{namespace mime{
	//////////////////////////////////////////////////////////////////////////
	HeaderAddress::HeaderAddress()
		: HeaderBase("HeaderAddress")
	{

	}
	
	//////////////////////////////////////////////////////////////////////////
	HeaderAddress::~HeaderAddress()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool HeaderAddress::construct(uintN argc, jsval *argv)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Mime.HeaderAddress must be called with 1 arg");
			return false;
		}

		char *value;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value)) return false;

		std::string valueOut;
		std::vector<jsval> addressesVector;

		using namespace boost::xpressive;

		static cregex reAddr = +(alnum|'.')>>'@'>>(+(alnum|'.'));

		static cregex _re = 
			(*_s)>>

			(
				(
					(s2=reAddr)|
					(as_xpr('"')>> (s1=+~as_xpr('"')) >> '"') |
					(s1=+(~(set= '<','>'))) |
					(nil)
				)>>
				(*_s)>>
				(
					(as_xpr('<')>>(s2=reAddr)>>'>') |
					(s2=reAddr)|
					(nil)
				)
			)>>

			(*(_s|','));
		cmatch _what;

		const char *iter = value;
		const char *end = value + strlen(value);
		while(boost::xpressive::regex_search(iter, end, _what, _re))
		{
			const char *sbeg = iter+_what.position();
			const char *send = sbeg+_what.length();
			iter = send;
			if(sbeg == send) break;

			std::string name = _what[1];
			trim(name);

			std::string addr = _what[2];
			if(addr.empty())continue;

			if(valueOut.size()) valueOut += ", ";
			if(name.size())
			{
				encodeNonAscii(name, true);
				valueOut += name;
				valueOut += ' ';
			}
			valueOut += '<';
			valueOut += addr;
			valueOut += '>';

			addressesVector.push_back(STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, addr.data(), addr.size())));
		}
		if(iter != end)
		{
			JS_ReportError(ecx()->_jsCx, "Mime.HeaderAddress unable to parse source '%s'", value);
			return false;
		}
		if(!jsRegisterProp("value", valueOut, true)) return false;

		JSObject *addressesObj;
		if(addressesVector.empty())
		{
			addressesObj = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);
		}
		else
		{
			addressesObj = JS_NewArrayObject(ecx()->_jsCx, addressesVector.size(), &addressesVector[0]);
		}
		if(!jsRegisterProp("addresses", OBJECT_TO_JSVAL(addressesObj), true)) return false;

		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool HeaderAddress::jsRegister()
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	HeaderAddressConstructor::HeaderAddressConstructor()
		: JsObject(true, "HeaderAddressConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool HeaderAddressConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		HeaderAddress *robj = new HeaderAddress;
		if(!robj->construct(argc, argv)) return false;
		*rval = robj->thisJsval();
		return true;
	}
}}
