#include "stdafx.h"
#include "mime/headers.hpp"
#include "mime/headerAddress.hpp"
#include "mime/headerComplex.hpp"
#include "mime/headerText.hpp"
#include "utils/string.hpp"

namespace ccms{namespace mime{

	//////////////////////////////////////////////////////////////////////////
	Headers::Headers()
		: JsObject(true, "[object Headers]")
	{
		//JSObject *proto = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);
		//(JSExceptionReporter)JS_SetPrototype(ecx()->_jsCx, _js, proto);
	}

	//////////////////////////////////////////////////////////////////////////
	Headers::~Headers()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Headers::construct(uintN argc, jsval *argv)
	{
		if(argc != 1)
		{
			JS_ReportError(ecx()->_jsCx, "Mime.headers must be called with 1 arg");
			return false;
		}

		if(!jsRegister()) return false;

		if(JSVAL_IS_OBJECT(argv[0]) && !JSVAL_IS_NULL(argv[0]))
		{
			JSObject *obj = JSVAL_TO_OBJECT(argv[0]);

			if(JS_IsArrayObject(ecx()->_jsCx, obj))
			{
				jsuint len;
				if(!JS_GetArrayLength(ecx()->_jsCx, obj, &len)) return false;
				for(jsuint i(0); i<len; i++)
				{
					jsval jsvValue;
					if(!JS_GetElement(ecx()->_jsCx, obj, i, &jsvValue)) return false;

					HeaderBase *value = parseHeader(jsvValue);
					if(!value)
					{
						return false;
					}
					if(!pushHeader(value))
					{
						return false;
					};
				}
			}
			else
			{
				JSIdArray *ids = JS_Enumerate(ecx()->_jsCx, obj);
				for(jsint i(0); i<ids->length; i++)
				{
					jsval jsvName;
					if(!JS_IdToValue(ecx()->_jsCx, ids->vector[i], &jsvName)) return false;
					char *name;
					if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsvName, "s", &name)) return false;

					jsval jsvValue;
					if(!JS_GetPropertyById(ecx()->_jsCx, obj, ids->vector[i], &jsvValue)) return false;

					HeaderBase *value = parseHeader(jsvValue, name);
					if(!value)
					{
						JS_DestroyIdArray(ecx()->_jsCx, ids);
						return false;
					}
					if(!pushHeader(value))
					{
						JS_DestroyIdArray(ecx()->_jsCx, ids);
						return false;
					};
				}
				JS_DestroyIdArray(ecx()->_jsCx, ids);
			}

		}
		else
		{
			//string Content-Type
			HeaderBase *value = parseHeader(argv[0], "Content-Type");
			if(!value)
			{
				return false;
			}

			if(!pushHeader(value)) return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Headers::jsRegister()
	{
		if(!jsRegisterMeth("value", &Headers::call_value, 0)) return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Headers::call_value(uintN argc, jsval *argv, jsval *rval)
	{
		std::vector<jsval> strings;
		jsval CRLF = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "\r\n", 2));
		jsval DDSP = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, ": ", 2));

		JSIdArray *ids = JS_Enumerate(ecx()->_jsCx, _js);
		jsval jsv;
		for(jsint i(0); i<ids->length; i++)
		{
			if(!JS_GetPropertyById(ecx()->_jsCx, _js, ids->vector[i], &jsv))
			{
				JS_DestroyIdArray(ecx()->_jsCx, ids);
				return false;
			}

			if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
			{
				JsObject *jso = JsObject::thisObj(JSVAL_TO_OBJECT(jsv));
				HeaderBase *ho = dynamic_cast<HeaderBase *>(jso);
				if(ho)
				{
					if(!JS_IdToValue(ecx()->_jsCx, ids->vector[i], &jsv))
					{
						JS_DestroyIdArray(ecx()->_jsCx, ids);
						return false;
					}

					strings.push_back(jsv);
					strings.push_back(DDSP);
					strings.push_back(jso->getProperty("value"));
					strings.push_back(CRLF);
				}
			}
		}

		JS_DestroyIdArray(ecx()->_jsCx, ids);

		if(strings.empty())
		{
			*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "", 0));
		}
		else
		{
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
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Headers::pushHeader(HeaderBase *value)
	{
		return JS_DefineProperty(ecx()->_jsCx, _js, value->getName(), value->thisJsval(), NULL, NULL, JSPROP_ENUMERATE)?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	HeaderBase *Headers::parseHeader(jsval jsvValue, const char *name)
	{
		if(name)
		{
				 if(ss_eq_nc("type", name)) name="Content-Type";
			else if(ss_eq_nc("disposition", name)) name="Content-Disposition";
			else if(ss_eq_nc("encoding", name)) name="Content-Transfer-Encoding";
			else if(ss_eq_nc("from", name)) name="From";
			else if(ss_eq_nc("to", name)) name="To";
			else if(ss_eq_nc("cc", name)) name="Cc";
			else if(ss_eq_nc("bcc", name)) name="Bcc";
			else if(ss_eq_nc("subject", name)) name="Subject";
			else if(ss_eq_nc("date", name)) name="Date";
			else if(ss_eq_nc("id", name)) name="Content-ID";
			else if(ss_eq_nc("cid", name)) name="Content-ID";

		}


		if(JSVAL_IS_OBJECT(jsvValue))
		{

			JsObject *jso = JsObject::thisObj(JSVAL_TO_OBJECT(jsvValue));

			HeaderBase *header = dynamic_cast<HeaderBase *>(jso);
			if(header)
			{
				if(!header->getName()[0])
				{
					if(!name)
					{
						JS_ReportError(ecx()->_jsCx, "Mime.Headers unable to determine header name");
						return NULL;
					}
					header->setName(name);
				}
				return header;
			}
		}


		if(!name)
		{
			if(!JSVAL_IS_OBJECT(jsvValue))
			{
				JS_ReportError(ecx()->_jsCx, "Mime.Headers unable to determine header name");
				return NULL;
			}

			JSObject *obj = JSVAL_TO_OBJECT(jsvValue);

			JSBool b;
			if(!JS_HasProperty(ecx()->_jsCx, obj, "name", &b)) return NULL;
			if(!b)
			{
				JS_ReportError(ecx()->_jsCx, "Mime.Headers object header must have 'name' property");
				return false;
			}

			jsval jsvName;
			if(!JS_GetProperty(ecx()->_jsCx, obj, "name", &jsvName)) return NULL;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsvName, "s", &name)) return NULL;

			if(!JS_HasProperty(ecx()->_jsCx, obj, "value", &b)) return NULL;
			if(b)
			{
				if(!JS_GetProperty(ecx()->_jsCx, obj, "value", &jsvValue)) return NULL;
			}
			else
			{
				//object itself will be used
			}
		}

		//HeaderAddress
		/*
			From
			To
			Cc
			Bcc
			Delivered-To
			Return-Path
		*/
		if(
			ss_eq_nc("from", name) ||
			ss_eq_nc("to", name) ||
			ss_eq_nc("cc", name) ||
			ss_eq_nc("bcc", name) ||
			ss_eq_nc("delivered-to", name) ||
			ss_eq_nc("return-path", name) ||
			0
			)
		{
			HeaderAddress *header = new HeaderAddress;
			if(!header->construct(1, &jsvValue)) return false;
			header->setName(name);
			return header;
		}

		//HeaderComplex
		/*
			Content-Type
			Content-Disposition
			Content-Transfer-Encoding
		*/
		if(
			ss_eq_nc("content-type", name) ||
			ss_eq_nc("content-disposition", name) ||
			ss_eq_nc("content-transfer-encoding", name) ||
			0
			)
		{
			HeaderComplex *header = new HeaderComplex;
			if(!header->construct(1, &jsvValue)) return false;
			header->setName(name);
			return header;
		}

		//HeaderText
		//все остальные
		{
			HeaderText *header = new HeaderText;
			if(!header->construct(1, &jsvValue)) return false;
			header->setName(name);
			return header;
		}

		return NULL;
	}



	//////////////////////////////////////////////////////////////////////////
	HeadersConstructor::HeadersConstructor()
		: JsObject(true, "[object HeadersConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool HeadersConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		Headers *robj = new Headers;
		if(!robj->construct(argc, argv)) return false;
		*rval = robj->thisJsval();
		return true;
	}
}}
