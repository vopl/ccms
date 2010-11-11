#include "stdafx.h"
#include "mime/file.hpp"
#include "mime/encoders.hpp"

namespace ccms{namespace mime{

	//////////////////////////////////////////////////////////////////////////
	File::File()
		: JsObject(true, "[object File]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	File::~File()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool File::construct(uintN argc, jsval *argv)
	{
		if(!jsRegister()) return false;

		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Mime.File must be called with 1 arg");
			return false;
		}

		if(!JS_SetReservedSlot(ecx()->_jsCx, _js, 0, argv[0])) return false;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool File::jsRegister()
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool File::call_value(uintN argc, jsval *argv, jsval *rval)
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

		std::string data;
		{
			std::ifstream in(value, std::ios::binary);
			if(!in)
			{
				JS_ReportError(ecx()->_jsCx, "File.value bad file name %s", value);
				return false;
			}
			in.seekg(0, std::ios::end);
			data.resize(in.tellg());
			char *buf = (char *)data.data();
			in.seekg(0, std::ios::beg);

			in.read(buf, data.size());
			in.close();
			jsval CRLF = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "\r\n", 2));

		}

		jsval CRLF = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "\r\n", 2));

		std::vector<jsval> strings;

		if(!encoding[0])
		{
			strings.push_back(jsv);
		}
		else if(!strcmp("7bit", encoding))
		{
			//7bit
			if(!encode_7bit(jsv, data.data(), data.size())) return false;
			strings.push_back(jsv);
		}
		else if(!strcmp("8bit", encoding))
		{
			//8bit
			//1000
			if(!encode_8bit(jsv, data.data(), data.size())) return false;
			strings.push_back(jsv);
		}
		else if(!strcmp("quoted-printable", encoding))
		{
			//quoted-printable
			if(!encode_qp(jsv, data.data(), data.size())) return false;
			strings.push_back(jsv);
		}
		else if(!strcmp("base64", encoding))
		{
			//base64
			if(!encode_base64(jsv, data.data(), data.size())) return false;
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
	FileConstructor::FileConstructor()
		: JsObject(true, "[object FileConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool FileConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		File *robj = new File;
		if(!robj->construct(argc, argv)) return false;
		*rval = robj->thisJsval();
		return true;
	}
}}
