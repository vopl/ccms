#include "stdafx.h"
#include "mime/content.hpp"
#include "mime/encoders.hpp"

namespace ccms{namespace mime{

	//////////////////////////////////////////////////////////////////////////
	Content::Content()
		: JsObject(true, "[object Content]")
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	Content::~Content()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Content::construct(uintN argc, jsval *argv)
	{
		if(!jsRegister()) return false;

		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Mime.Content must be called with 1 arg");
			return false;
		}

		if(!JS_SetReservedSlot(ecx()->_jsCx, _js, 0, argv[0])) return false;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Content::jsRegister()
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Content::call_value(uintN argc, jsval *argv, jsval *rval)
	{
		const char *encoding = "";
		if(argc>=1)
		{
			if(!JSVAL_IS_NULL(argv[0]) && !JSVAL_IS_VOID(argv[0]))
			{
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &encoding)) return false;
			}
		}

		jsval jsv;
		if(!JS_GetReservedSlot(ecx()->_jsCx, _js, 0, &jsv)) return false;

		const char *value;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &value)) return false;

		jsval CRLF = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "\r\n", 2));

		std::vector<jsval> strings;

		if(!encoding[0])
		{
			strings.push_back(jsv);
		}
		else if(!strcmp("7bit", encoding))
		{
			//7bit
			if(!encode_7bit(jsv)) return false;
			strings.push_back(jsv);
		}
		else if(!strcmp("8bit", encoding))
		{
			//8bit
			//1000
			if(!encode_8bit(jsv)) return false;
			strings.push_back(jsv);
		}
		else if(!strcmp("quoted-printable", encoding))
		{
			//quoted-printable
			if(!encode_qp(jsv)) return false;
			strings.push_back(jsv);
		}
		else if(!strcmp("base64", encoding))
		{
			//base64
			if(!encode_base64(jsv)) return false;
			strings.push_back(jsv);
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "Content.value unknown encoding: %s", encoding);
			return false;
		}
		strings.push_back(CRLF);

		if(strings.size()>1)
		{
			JSObject *stro;
			if(!JS_ValueToObject(ecx()->_jsCx, strings[0], &stro)) return false;
			if(!JS_CallFunctionName(ecx()->_jsCx, stro, "concat", strings.size()-1, &strings[1], rval)) return false;
		}
		else
		{
			*rval = strings[0];
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	ContentConstructor::ContentConstructor()
		: JsObject(true, "[object ContentConstructor]")
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	JSBool ContentConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		Content *robj = new Content;
		if(!robj->construct(argc, argv)) return false;
		*rval = robj->thisJsval();
		return true;
	}
}}
