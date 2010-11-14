#include "stdafx.h"
#include "mime/part.hpp"
#include "mime/headers.hpp"
#include "mime/content.hpp"
#include "mime/file.hpp"
#include "mime/headerComplex.hpp"
#include "mime/utils.hpp"

namespace ccms{namespace mime{

	//////////////////////////////////////////////////////////////////////////
	Part::Part()
		: JsObject(true, "Part")
		, _contentType(ectNull)
	{
		_content._raw = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	Part::~Part()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Part::construct(uintN argc, jsval *argv)
	{
		if(argc > 2)
		{
			JS_ReportError(ecx()->_jsCx, "Mime.Part must be called with 0, 1 or 2 args");
			return false;
		}

		if(!jsRegister()) return false;

		if(argc >= 1)
		{
			if(!xetter_headers(&argv[0], false)) return false;
		}
		if(argc >= 2)
		{
			if(!xetter_content(&argv[1], false)) return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Part::jsRegister()
	{
		if(!jsRegisterProp("headers", &Part::xetter_headers, false)) return false;
		if(!jsRegisterProp("content", &Part::xetter_content, false)) return false;

		if(!jsRegisterMeth("value", &Part::call_value, 0)) return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Part::xetter_headers(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(_headers)
			{
				*vp = _headers->thisJsval();
			}
			else
			{
				*vp = JSVAL_NULL;
			}
		}
		else
		{
			Headers *headers = NULL;
			if(JSVAL_IS_OBJECT(*vp) && !JSVAL_IS_NULL(*vp))
			{
				JsObject *jso = JsObject::thisObj(JSVAL_TO_OBJECT(*vp));
				headers = dynamic_cast<Headers *>(jso);
			}

			if(!headers)
			{
				headers = new Headers;
				if(!headers->construct(1, vp)) return false;
			}

			_headers = headers;
			JS_SetReservedSlot(ecx()->_jsCx, _js, 0, _headers->thisJsval());
		}
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Part::xetter_content(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			switch(_contentType)
			{
			case ectNull:
				*vp = JSVAL_NULL;
				break;
			case ectRaw:
				*vp = _content._raw->thisJsval();
				break;
			case ectFile:
				*vp = _content._file->thisJsval();
				break;
			case ectPart:
				*vp = _content._part->thisJsval();
				break;
			case ectArray:
				*vp = OBJECT_TO_JSVAL(_content._array);
				break;
			default:
				assert(!"wtf?");
				*vp = JSVAL_NULL;
				return true;
			}
		}
		else
		{
			EContentType ect = ectNull;
			if(JSVAL_IS_OBJECT(*vp) && !JSVAL_IS_NULL(*vp))
			{
				JSObject *obj = JSVAL_TO_OBJECT(*vp);
				if(JS_IsArrayObject(ecx()->_jsCx, obj))
				{
					ect = ectArray;
					_content._array = obj;
					JS_SetReservedSlot(ecx()->_jsCx, _js, 1, *vp);
				}
				else
				{
					JsObject *jso = JsObject::thisObj(obj);
					Part *part = dynamic_cast<Part *>(jso);
					if(part)
					{
						ect = ectPart;
						_content._part = part;
						JS_SetReservedSlot(ecx()->_jsCx, _js, 1, _content._part->thisJsval());
					}
					else
					{
						File *file = dynamic_cast<File *>(jso);
						if(file)
						{
							ect = ectFile;
							_content._file = file;
							JS_SetReservedSlot(ecx()->_jsCx, _js, 1, _content._file->thisJsval());
						}
						else
						{
							Content *raw = dynamic_cast<Content *>(jso);
							if(raw)
							{
								ect = ectRaw;
								_content._raw = raw;
								JS_SetReservedSlot(ecx()->_jsCx, _js, 1, _content._raw->thisJsval());
							}
							else
							{
								//new raw, keep ect=ectNull
							}
						}
					}
				}

			}

			if(ectNull == ect)
			{
				Content *raw = new Content;
				if(!raw->construct(1, vp)) return false;
				ect = ectRaw;
				_content._raw = raw;
				JS_SetReservedSlot(ecx()->_jsCx, _js, 1, _content._raw->thisJsval());
			}
			_contentType = ect;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Part::call_value(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = JSVAL_VOID;
		if(_headers)
		{
			std::vector<jsval> strings;

			jsval jsv;

			//вы€снить по заголовкам encoding
			//headers."Content-Transfer-Encoding".parameters.""
			jsval encoding = JSVAL_VOID;
			if(!JS_GetProperty(ecx()->_jsCx, _headers->thisJsobj(), "Content-Transfer-Encoding", &jsv)) return false;
			if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
			{
				JSObject *obj = JSVAL_TO_OBJECT(jsv);
				if(!JS_GetProperty(ecx()->_jsCx, obj, "parameters", &jsv)) return false;
				if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
				{
					JSObject *obj = JSVAL_TO_OBJECT(jsv);
					if(!JS_GetProperty(ecx()->_jsCx, obj, "", &encoding)) return false;
				}
			}
			//вы€снить по заголовкам мультипарт или нет
			//headers."Content-Type".parameters.""
			const char *hct = "";
			const char *hctb = "";
			if(!JS_GetProperty(ecx()->_jsCx, _headers->thisJsobj(), "Content-Type", &jsv)) return false;
			if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
			{
				JSObject *obj = JSVAL_TO_OBJECT(jsv);
				if(!JS_GetProperty(ecx()->_jsCx, obj, "parameters", &jsv)) return false;
				if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
				{
					JSObject *obj = JSVAL_TO_OBJECT(jsv);
					if(!JS_GetProperty(ecx()->_jsCx, obj, "", &jsv)) return false;
					if(JSVAL_IS_STRING(jsv))
					{
						if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &hct)) return false;
					}

					if(!JS_GetProperty(ecx()->_jsCx, obj, "boundary", &jsv)) return false;
					if(JSVAL_IS_STRING(jsv))
					{
						if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &hctb)) return false;
					}
				}
			}


			bool isMultipart = 0==strncmp("multipart", hct, 9);

			std::string boundary;
			if(isMultipart)
			{
				if(!hctb[0])
				{
					//добавить границу
					if(!JS_GetProperty(ecx()->_jsCx, _headers->thisJsobj(), "Content-Type", &jsv)) return false;
					JSObject *obj = JSVAL_TO_OBJECT(jsv);
					if(!JS_GetProperty(ecx()->_jsCx, obj, "value", &jsv)) return false;

					boundary = genBoundary();
					jsv = STRING_TO_JSVAL(
							JS_ConcatStrings(
								ecx()->_jsCx, 
								JSVAL_TO_STRING(jsv), 
								JS_NewStringCopyZ(
									ecx()->_jsCx, (std::string("; boundary=")+boundary).data())
							)
					);

					HeaderComplex *newHeader = new HeaderComplex;
					if(!newHeader->construct(1, &jsv)) return false;

					if(!_headers->jsRegisterProp("Content-Type", newHeader->thisJsval(), true)) return false;
				}
				else
				{
					boundary = hctb;
				}

				//print headers
				strings.push_back(JSVAL_VOID);
				if(!_headers->call_value(0, NULL, &strings.back())) return false;

				//print headers-content delimiter
				jsval CRLF = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "\r\n", 2));
				strings.push_back(CRLF);

				jsval BND = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, boundary.data(), boundary.size()));
				jsval MIMI = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "--", 2));

				strings.push_back(MIMI);
				strings.push_back(BND);

				//content
				switch(_contentType)
				{
				case ectNull:
					strings.push_back(MIMI);
					strings.push_back(CRLF);
					break;

				case ectRaw:
					strings.push_back(CRLF);

					if(!_content._raw->call_value(1, &encoding, &jsv)) return false;
					strings.push_back(jsv);

					strings.push_back(MIMI);
					strings.push_back(BND);
					strings.push_back(MIMI);
					strings.push_back(CRLF);
					break;

				case ectFile:
					strings.push_back(CRLF);

					if(!_content._file->call_value(1, &encoding, &jsv)) return false;
					strings.push_back(jsv);

					strings.push_back(MIMI);
					strings.push_back(BND);
					strings.push_back(MIMI);
					strings.push_back(CRLF);
					break;

				case ectPart:
					strings.push_back(CRLF);

					if(!_content._raw->call_value(0, NULL, &jsv)) return false;
					strings.push_back(jsv);

					strings.push_back(MIMI);
					strings.push_back(BND);
					strings.push_back(MIMI);
					strings.push_back(CRLF);
					break;

				case ectArray:
					{
						jsuint len;
						if(!JS_GetArrayLength(ecx()->_jsCx, _content._array, &len)) return false;
						for(size_t i(0); i<len; i++)
						{

							strings.push_back(CRLF);

							if(!JS_GetElement(ecx()->_jsCx, _content._array, i, &jsv)) return false;
							if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
							{
								JSObject *obj = JSVAL_TO_OBJECT(jsv);
								if(!JS_CallFunctionName(ecx()->_jsCx, obj, "value", 0, NULL, &jsv)) return false;
							}
							strings.push_back(jsv);

							//part
							strings.push_back(MIMI);
							strings.push_back(BND);
						}
						strings.push_back(MIMI);
						strings.push_back(CRLF);
					}
					break;
				default:
					assert(!"wtf?");
					strings.push_back(MIMI);
					strings.push_back(BND);
					strings.push_back(MIMI);
					strings.push_back(CRLF);
					return true;
				}

			}
			else
			{
				//print headers
				strings.push_back(JSVAL_VOID);
				if(!_headers->call_value(0, NULL, &strings.back())) return false;

				//print headers-content delimiter
				jsval CRLF = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "\r\n", 2));
				strings.push_back(CRLF);

				//content
				switch(_contentType)
				{
				case ectNull:
					break;

				case ectRaw:
					if(!_content._raw->call_value(1, &encoding, &jsv)) return false;
					strings.push_back(jsv);
					break;

				case ectFile:
					if(!_content._file->call_value(1, &encoding, &jsv)) return false;
					strings.push_back(jsv);
					break;

				case ectPart:
					if(!_content._raw->call_value(0, NULL, &jsv)) return false;
					strings.push_back(jsv);
					break;

				case ectArray:
					{
						jsuint len;
						if(!JS_GetArrayLength(ecx()->_jsCx, _content._array, &len)) return false;
						for(size_t i(0); i<len; i++)
						{

							if(!JS_GetElement(ecx()->_jsCx, _content._array, i, &jsv)) return false;
							if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
							{
								JSObject *obj = JSVAL_TO_OBJECT(jsv);
								if(!JS_CallFunctionName(ecx()->_jsCx, obj, "value", 0, NULL, &jsv)) return false;
							}
							strings.push_back(jsv);
						}
					}
					break;
				default:
					assert(!"wtf?");
					return true;
				}
			}

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
	bool Part::getFromAll(std::string &data)
	{
		jsval jsv;
		if(!JS_GetProperty(ecx()->_jsCx, _headers->thisJsobj(), "From", &jsv)) return false;
		if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
		{
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			if(!JS_GetProperty(ecx()->_jsCx, obj, "addresses", &jsv)) return false;
			if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
			{
				JSObject *arr = JSVAL_TO_OBJECT(jsv);

				if(!JS_CallFunctionName(ecx()->_jsCx, arr, "join", 0, NULL, &jsv)) return false;

				char *res;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &res)) return false;

				data = res;
			}
		}

		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Part::getToAll(std::string &data)
	{
		JSObject *arrAll = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);

		jsval jsv;
		//////////////////////////////////////////////////////////////////////////
		if(!JS_GetProperty(ecx()->_jsCx, _headers->thisJsobj(), "To", &jsv)) return false;
		if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
		{
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			if(!JS_GetProperty(ecx()->_jsCx, obj, "addresses", &jsv)) return false;
			if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
			{
				if(!JS_CallFunctionName(ecx()->_jsCx, arrAll, "push", 1, &jsv, &jsv)) return false;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		if(!JS_GetProperty(ecx()->_jsCx, _headers->thisJsobj(), "Cc", &jsv)) return false;
		if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
		{
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			if(!JS_GetProperty(ecx()->_jsCx, obj, "addresses", &jsv)) return false;
			if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
			{
				if(!JS_CallFunctionName(ecx()->_jsCx, arrAll, "push", 1, &jsv, &jsv)) return false;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		if(!JS_GetProperty(ecx()->_jsCx, _headers->thisJsobj(), "Bcc", &jsv)) return false;
		if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
		{
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			if(!JS_GetProperty(ecx()->_jsCx, obj, "addresses", &jsv)) return false;
			if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
			{
				if(!JS_CallFunctionName(ecx()->_jsCx, arrAll, "push", 1, &jsv, &jsv)) return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		if(!JS_CallFunctionName(ecx()->_jsCx, arrAll, "join", 0, NULL, &jsv)) return false;

		char *res;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &res)) return false;
		data = res;
		return true;

	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Part::getData(std::string &data)
	{
		jsval jsv;
		if(!call_value(0, NULL, &jsv)) return false;
		char *res;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &res)) return false;
		data = res;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	PartConstructor::PartConstructor()
		: JsObject(true, "PartConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PartConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		Part *robj = new Part;
		if(!robj->construct(argc, argv)) return false;
		*rval = robj->thisJsval();
		return true;
	}
}}
