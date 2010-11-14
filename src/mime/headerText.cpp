#include "stdafx.h"
#include "mime/headerText.hpp"
#include "utils/string.hpp"
#include "mime/utils.hpp"

namespace ccms{namespace mime{
	//////////////////////////////////////////////////////////////////////////
	HeaderText::HeaderText()
		: HeaderBase("HeaderText")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	HeaderText::~HeaderText()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool HeaderText::construct(uintN argc, jsval *argv)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Mime.HeaderText must be called with 1 arg");
			return false;
		}

		char *value;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value)) return false;
		std::string valOut = value;
		trim(valOut);
		encodeNonAscii(valOut);
		if(!jsRegisterProp("value", valOut, true)) return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool HeaderText::jsRegister()
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	HeaderTextConstructor::HeaderTextConstructor()
		: JsObject(true, "HeaderTextConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool HeaderTextConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		HeaderText *robj = new HeaderText;
		if(!robj->construct(argc, argv)) return false;
		*rval = robj->thisJsval();
		return true;
	}
}}
