#include "stdafx.h"
#include "mime/headerComplex.hpp"
#include "utils/string.hpp"
#include "mime/utils.hpp"

namespace ccms{namespace mime{
	//////////////////////////////////////////////////////////////////////////
	HeaderComplex::HeaderComplex()
		: HeaderBase("HeaderComplex")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	HeaderComplex::~HeaderComplex()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool HeaderComplex::construct(uintN argc, jsval *argv)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Mime.HeaderComplex must be called with 1 arg");
			return false;
		}

		char *value;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value)) return false;

		std::string valueOut;

		JSObject *valuesObj = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);

		using namespace boost::xpressive;
		static cregex _re1 = *_s >> (s1=+(~as_xpr(';'))) >>*_s;
		static cregex _re2 = *_s >> as_xpr(';') >> *_s >> (s1=+(~as_xpr('='))) >> *_s >> ((as_xpr('=') >> *_s >> (s2=+(~as_xpr(';'))))|nil);

		cmatch _what;
		const char *iter = value;
		const char *end = value + strlen(value);
		if(boost::xpressive::regex_search(iter, end, _what, _re1))
		{
			const char *sbeg = iter+_what.position();
			const char *send = sbeg+_what.length();
			iter = send;
			//if(sbeg == send) break;

			std::string valMain = _what[1];
			trim(valMain);
			if(!JS_DefineProperty(
				ecx()->_jsCx, 
				valuesObj, 
				"", 
				STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, valMain.data(), valMain.size())),
				NULL, NULL, JSPROP_ENUMERATE)) return false;
			encodeNonAscii(valMain);

			valueOut += valMain;
		}

		while(boost::xpressive::regex_search(iter, end, _what, _re2))
		{
			const char *sbeg = iter+_what.position();
			const char *send = sbeg+_what.length();
			iter = send;
			if(sbeg == send) break;

			std::string valKey = _what[1];
			trim(valKey);

			std::string valVal = _what[2];
			trim(valVal);

			if(!JS_DefineProperty(
				ecx()->_jsCx, 
				valuesObj, 
				valKey.data(), 
				STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, valVal.data(), valVal.size())),
				NULL, NULL, JSPROP_ENUMERATE)) return false;

			encodeNonAscii(valKey);
			encodeNonAscii(valVal);

			valueOut += "; ";
			valueOut += valKey;
			valueOut += "=";
			valueOut += valVal;
		}

		if(iter != end)
		{
			JS_ReportError(ecx()->_jsCx, "Mime.HeaderComplex unable to parse source '%s'", value);
			return false;
		}
		if(!jsRegisterProp("value", valueOut, true)) return false;
		if(!jsRegisterProp("parameters", OBJECT_TO_JSVAL(valuesObj), true)) return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool HeaderComplex::jsRegister()
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	HeaderComplexConstructor::HeaderComplexConstructor()
		: JsObject(true, "HeaderComplexConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool HeaderComplexConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		HeaderComplex *robj = new HeaderComplex;
		if(!robj->construct(argc, argv)) return false;
		*rval = robj->thisJsval();
		return true;
	}
}}
