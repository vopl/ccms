#include "stdafx.h"
#include "tpm/templateSource.hpp"
#include "tpm/template.hpp"
#include "router/execContext.hpp"

namespace ccms
{

	//////////////////////////////////////////////////////////////////////////
	TemplateSource::TemplateSource(Template *owner)
		: _owner(owner)
		, _etet(etetNull)
		, _type(etstNull)
	{
		_content._text._data = NULL;
		_content._text._size = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	TemplateSource::~TemplateSource()
	{
		clearContent();
	}

	//////////////////////////////////////////////////////////////////////////
	void TemplateSource::clearContent()
	{
		switch(_type)
		{
		case etstNull:
			break;
		case etstText:
			free(_content._text._data);
			break;
		case etstJsval:
// 			if(JSVAL_IS_GCTHING(_content._jsval)) JS_RemoveRoot(ecx()->_jsCx, &_content._jsval);
			if(JSVAL_IS_GCTHING(_content._jsval)) _owner->unclenchJsval(this, 0);
			break;
		case etstProp:
// 			if(JSVAL_IS_GCTHING(_content._prop._idVal)) JS_RemoveRoot(ecx()->_jsCx, &_content._prop._idVal);
// 			if(JSVAL_IS_GCTHING(_content._prop._val)) JS_RemoveRoot(ecx()->_jsCx, &_content._prop._val);
			if(JSVAL_IS_GCTHING(_content._prop._idVal))_owner->unclenchJsval(this, 0);
			if(JSVAL_IS_GCTHING(_content._prop._val))_owner->unclenchJsval(this, 1);
			break;
		default:
			assert(!"unknown templateSourceType");
		}
		_type = etstNull;
	}

	//////////////////////////////////////////////////////////////////////////
	void TemplateSource::clearContentNoJs()
	{
		switch(_type)
		{
		case etstNull:
			break;
		case etstText:
			free(_content._text._data);
			break;
		case etstJsval:
			break;
		case etstProp:
			break;
		default:
			assert(!"unknown templateSourceType");
		}
		_type = etstNull;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TemplateSource::setText(const char *data, size_t size)
	{
		clearContent();
		_content._text._size = size;
		_content._text._data = (char *)malloc(_content._text._size);
		strncpy(_content._text._data, data, _content._text._size);
		_type = etstText;
		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	bool TemplateSource::setJsval(jsval jsv, bool doSimplify)
	{
		clearContent();

		if(doSimplify)
		{
			JSType jst = JS_TypeOfValue(ecx()->_jsCx, jsv);
			switch(jst)
			{
			case JSTYPE_XML:
				{
					if(!JS_CallFunctionName(ecx()->_jsCx, JSVAL_TO_OBJECT(jsv), "toXMLString", 0, NULL, &jsv))
					{
						return false;
					}
					return setJsval(jsv, false);
				}
			case JSTYPE_VOID:
				return true;
			case JSTYPE_STRING:
			case JSTYPE_NUMBER:
			case JSTYPE_BOOLEAN:
			case JSTYPE_NULL:
			case JSTYPE_LIMIT:
				{
					char *data;
					if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &data))
					{
						return false;
					}
					size_t size = strlen(data);

					_content._text._size = size;
					_content._text._data = (char *)malloc(_content._text._size);
					strncpy(_content._text._data, data, _content._text._size);
					_type = etstText;
					return true;
				}
			case JSTYPE_OBJECT:
			case JSTYPE_FUNCTION:
				{
					_content._jsval = jsv;
// 					if(JSVAL_IS_GCTHING(_content._jsval)) JS_AddNamedRoot(ecx()->_jsCx, &_content._jsval);
					if(JSVAL_IS_GCTHING(_content._jsval)) _owner->clenchJsval(this, 0, _content._jsval);

					_type = etstJsval;
					return true;
				}
			default:
				assert(0);
				return false;
			}
		}

		_content._jsval = jsv;
// 		if(JSVAL_IS_GCTHING(_content._jsval)) JS_AddNamedRoot(ecx()->_jsCx, &_content._jsval, __FUNCTION__);
		if(JSVAL_IS_GCTHING(_content._jsval)) _owner->clenchJsval(this, 0, _content._jsval);

		_type = etstJsval;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TemplateSource::setProp(jsval id, jsval val)
	{
		clearContent();

		jsid jid;
		if(!JS_ValueToId(ecx()->_jsCx, id, &jid))
		{
			assert(0);
			return false;
		}

		_content._prop._idVal = id;
		_content._prop._id = jid;
		_content._prop._val = val;

// 		if(JSVAL_IS_GCTHING(_content._prop._idVal)) JS_AddNamedRoot(ecx()->_jsCx, &_content._prop._idVal, __FUNCTION__);
// 		if(JSVAL_IS_GCTHING(_content._prop._val)) JS_AddNamedRoot(ecx()->_jsCx, &_content._prop._val, __FUNCTION__);

		if(JSVAL_IS_GCTHING(_content._prop._idVal)) _owner->clenchJsval(this, 0, _content._prop._idVal);
		if(JSVAL_IS_GCTHING(_content._prop._val)) _owner->clenchJsval(this, 1, _content._prop._val);

		_type = etstProp;
		return true;

	}

	//////////////////////////////////////////////////////////////////////////
	bool TemplateSource::getJsval(JSObject *obj, jsval &jsv)
	{
		switch(_type)
		{
		case etstNull:
			jsv = JSVAL_VOID;
			break;
		case etstText:
			{
				JSString *str = JS_NewStringCopyN(ecx()->_jsCx, _content._text._data, _content._text._size);
				if(!str)
				{
					return false;
				}
				jsv = STRING_TO_JSVAL(str);
			}
			break;
		case etstJsval:
			jsv = _content._jsval;
			break;
		case etstProp:

			if(!obj)
			{
				jsv = _content._prop._val;
				return true;
			}
			{
				JSBool b;
				if(!JS_AlreadyHasOwnPropertyById(ecx()->_jsCx, obj, _content._prop._id, &b))
				{
					return false;
				}

				if(!b)
				{
					jsv = _content._prop._val;
					return true;
				}
			}
			
			if(!JS_GetPropertyById(ecx()->_jsCx, obj, _content._prop._id, &jsv))
			{
				return false;
			}
			break;
		default:
			assert(!"unknown type");
			break;
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	void TemplateSource::setEtet(ETemplateEscaperType etet)
	{
		_etet = etet;
	}

	//////////////////////////////////////////////////////////////////////////
	TemplateSourcePtr TemplateSource::clone()
	{
		TemplateSourcePtr res(new TemplateSource(_owner));
		res->setEtet(_etet);
		switch(_type)
		{
		case etstNull:
			break;
		case etstText:
			res->setText(_content._text._data, _content._text._size);
			break;
		case etstJsval:
			res->setJsval(_content._jsval, false);
			break;
		case etstProp:
			res->setProp(_content._prop._idVal, _content._prop._val);
			break;
		default:
			assert(!"unknown type");
			break;
		}

		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TemplateSource::print(JSObject *obj, std::ostream &out, const TemplateEscaper &escaper)
	{

		TemplateEscaper thisEscaper(_etet, &escaper);
		const TemplateEscaper &useEscaper = etetNull==_etet?escaper:thisEscaper;

		switch(_type)
		{
		case etstNull:
			assert(0);
			return true;
		case etstText:
			return useEscaper.print(out, _content._text._data, _content._text._size);
		case etstJsval:
			return printJsval(obj, out, useEscaper, _content._jsval);
		case etstProp:
			{
				jsval jsv;
				if(!JS_GetPropertyById(ecx()->_jsCx, obj, _content._prop._id, &jsv))return false;
				return printJsval(obj, out, useEscaper, jsv);
			}
		default:
			assert(!"unknown templateSourceType");
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TemplateSource::less(const TemplateSource &with) const
	{
		if(_etet < with._etet) return true;
		if(_etet > with._etet) return false;

		if(_type < with._type) return true;
		if(_type > with._type) return false;

		switch(_type)
		{
		case etstNull:
			return false;
		case etstText:
			if(_content._text._size < with._content._text._size) return true;
			if(_content._text._size > with._content._text._size) return false;
			return strncmp(_content._text._data, with._content._text._data, _content._text._size)<0;
		case etstJsval:
			return _content._jsval < with._content._jsval;
		case etstProp:
			return _content._prop._id < with._content._prop._id;
		default:
			assert(0);
			return false;
		}

		assert(0);
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TemplateSource::printJsval(JSObject *obj_, std::ostream &out, const TemplateEscaper &escaper, jsval jsv)
	{
		JSType jst = JS_TypeOfValue(ecx()->_jsCx, jsv);
		switch(jst)
		{
		case JSTYPE_XML:
			{
				if(!JS_CallFunctionName(ecx()->_jsCx, JSVAL_TO_OBJECT(jsv), "toXMLString", 0, NULL, &jsv))
				{
					return false;
				}

				char *data;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &data))
				{
					return false;
				}
				size_t size = strlen(data);

				if(etetXml == escaper.getType() && etetXml ==_etet)
				{
					//already xml, do not apply escaper
					if(escaper.getTail())
					{
						return escaper.getTail()->print(out, data, size);
					}
					return TemplateEscaper(etetNull).print(out, data, size);
				}
				return escaper.print(out, data, size);
			}
		case JSTYPE_VOID:
			return true;
		case JSTYPE_STRING:
		case JSTYPE_NUMBER:
		case JSTYPE_BOOLEAN:
		case JSTYPE_NULL:
		case JSTYPE_LIMIT:
			{
				char *s;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &s))
				{
					return false;
				}

				return escaper.print(out, s, strlen(s));
			}
			assert(0);
			return false;

		case JSTYPE_OBJECT:
		case JSTYPE_FUNCTION:
			if(!JSVAL_IS_NULL(jsv))
			{
				JSObject *objv = JSVAL_TO_OBJECT(jsv);

				if(JS_IsArrayObject(ecx()->_jsCx, objv))
				{
					jsuint len;
					if(!JS_GetArrayLength(ecx()->_jsCx, objv, &len)) return false;
					for(jsuint i(0); i<len; i++)
					{
						jsval jsv;
						if(!JS_GetElement(ecx()->_jsCx, objv, i, &jsv)) return false;
						if(!printJsval(obj_, out, escaper, jsv)) return false;
					}
					return true;
				}


				//если объект Template или унаследованный от него
				{
					JSObject *objvt = objv;
					while(objvt)
					{
						JsObject *jso = JsObject::thisObj(objvt);
						if(jso)
						{
							Template *tjso = dynamic_cast<Template *>(jso);
							if(tjso)
							{
								if(ets_xml == tjso->getState() && etetXml == escaper.getType() && etetXml == _etet)
								{
									if(escaper.getTail())
									{
										if(!tjso->print(objv, out, *escaper.getTail())) return false;
									}
									else
									{
										TemplateEscaper nullEscaper(etetNull);
										if(!tjso->print(objv, out, nullEscaper)) return false;
									}
								}
								else
								{
									if(!tjso->print(objv, out, escaper)) return false;
								}

								return true;
							}
						}
						objvt = JS_GetPrototype(ecx()->_jsCx, objvt);
					}

				}
				JSBool found;

				//if can print itself
				if(JS_HasProperty(ecx()->_jsCx, objv, "print", &found) && found)
				{
					jsval jsvf;
					if(!JS_GetProperty(ecx()->_jsCx, objv, "print", &jsvf)) return false;

					if(JSTYPE_FUNCTION != JS_TypeOfValue(ecx()->_jsCx, jsvf))
					{
						JS_ReportError(ecx()->_jsCx, "TemplateSource: 'print' is not a function");
						return false;
					}

					jsval rval;
					if(!JS_CallFunctionValue(ecx()->_jsCx, objv, jsvf, 0, NULL, &rval))
					{
						return false;
					}
					//return printJsval(out, escaper, rval);
					return true;
				}

				if(JS_HasProperty(ecx()->_jsCx, objv, "render", &found) && found)
				{
					jsval jsvf;
					if(!JS_GetProperty(ecx()->_jsCx, objv, "render", &jsvf)) return false;

					if(JSTYPE_FUNCTION != JS_TypeOfValue(ecx()->_jsCx, jsvf))
					{
						JS_ReportError(ecx()->_jsCx, "TemplateSource: 'render' is not a function");
						return false;
					}

					jsval rval;
					if(!JS_CallFunctionValue(ecx()->_jsCx, objv, jsvf, 0, NULL, &rval))
					{
						return false;
					}
					return printJsval(obj_, out, escaper, rval);
				}

				if(JSTYPE_FUNCTION == jst)
				{//real function
					jsval rval;
					if(!JS_CallFunctionValue(ecx()->_jsCx, obj_, jsv, 0, NULL, &rval))
					{
						return false;
					}
					return printJsval(obj_, out, escaper, rval);
				}
				else
				{//object
					//to string
					char *s;
					if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &s))
					{
						return false;
					}
					return escaper.print(out, s, strlen(s));
				}
			}
			else
			{
				return escaper.print(out, "null", 4);
			}
			assert(0);
			return false;

		default:
			assert(0);
			return false;
		}

		return false;
	}

}
