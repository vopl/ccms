#include "magick_pch.h"
#include "magick/geometry.hpp"
#include "magick/macroHelpers.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	Geometry::Geometry(uint32 width_,
		uint32 height_,
		int32 xOff_,
		int32 yOff_,
		bool xNegative_,
		bool yNegative_)
		: JsObject(true, "[object Geometry]")
		, _impl(new ::Magick::Geometry(width_, height_, xOff_, yOff_, xNegative_, yNegative_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	Geometry::Geometry(const char * geometry_)
		: JsObject(true, "[object Geometry]")
		, _impl(new ::Magick::Geometry(geometry_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	Geometry::Geometry(const Geometry &geometry_)
		: JsObject(true, "[object Geometry]")
		, _impl(new ::Magick::Geometry(geometry_.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	Geometry::Geometry(const ::Magick::Geometry &geometry_)
		: JsObject(true, "[object Geometry]")
		, _impl(new ::Magick::Geometry(geometry_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	Geometry::Geometry()
		: JsObject(true, "[object Geometry]")
		, _impl(new ::Magick::Geometry)
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	Geometry::~Geometry()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_width(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->width());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->width(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_height(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->height());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->height(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_xOff(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->xOff());
		}
		else
		{
			int32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "i", &v)) return false;
			_impl->xOff(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_yOff(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->yOff());
		}
		else
		{
			int32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "i", &v)) return false;
			_impl->yOff(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_xNegative(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = BOOLEAN_TO_JSVAL(_impl->xNegative());
		}
		else
		{
			JSBool v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "b", &v)) return false;
			_impl->xNegative(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_yNegative(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = BOOLEAN_TO_JSVAL(_impl->yNegative());
		}
		else
		{
			JSBool v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "b", &v)) return false;
			_impl->yNegative(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_percent(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = BOOLEAN_TO_JSVAL(_impl->percent());
		}
		else
		{
			JSBool v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "b", &v)) return false;
			_impl->percent(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_aspect(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = BOOLEAN_TO_JSVAL(_impl->aspect());
		}
		else
		{
			JSBool v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "b", &v)) return false;
			_impl->aspect(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_greater(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = BOOLEAN_TO_JSVAL(_impl->greater());
		}
		else
		{
			JSBool v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "b", &v)) return false;
			_impl->greater(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_less(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = BOOLEAN_TO_JSVAL(_impl->less());
		}
		else
		{
			JSBool v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "b", &v)) return false;
			_impl->less(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_isValid(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = BOOLEAN_TO_JSVAL(_impl->isValid());
		}
		else
		{
			JSBool v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "b", &v)) return false;
			_impl->isValid(v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::xetter_string(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			IMTRY()
				std::string s = *_impl;
				*vp = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, s.data(), s.size()));
			IMCATCH(*vp = STRING_TO_JSVAL(JS_NewStringCopyZ(ecx()->_jsCx, "value acquiring failed")))

		}
		else
		{
			char *v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &v)) return false;

			IMTRY()
				*_impl = v;
			IMCATCH(return false)
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::Geometry &Geometry::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Geometry::registerJs()
	{
		if(!jsRegisterProp("width", &Geometry::xetter_width, false))return false;
		if(!jsRegisterProp("height", &Geometry::xetter_height, false))return false;
		if(!jsRegisterProp("xOff", &Geometry::xetter_xOff, false))return false;
		if(!jsRegisterProp("yOff", &Geometry::xetter_yOff, false))return false;
		if(!jsRegisterProp("xNegative", &Geometry::xetter_xNegative, false))return false;
		if(!jsRegisterProp("yNegative", &Geometry::xetter_yNegative, false))return false;
		if(!jsRegisterProp("percent", &Geometry::xetter_percent, false))return false;
		if(!jsRegisterProp("aspect", &Geometry::xetter_aspect, false))return false;
		if(!jsRegisterProp("greater", &Geometry::xetter_greater, false))return false;
		if(!jsRegisterProp("less", &Geometry::xetter_less, false))return false;
		if(!jsRegisterProp("isValid", &Geometry::xetter_isValid, false))return false;
		if(!jsRegisterProp("string", &Geometry::xetter_string, false))return false;

		return true;
	}











	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	GeometryConstructor::GeometryConstructor()
		: JsObject(true, "[object GeometryConstructor]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool GeometryConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(0 == argc)
		{
			*rval = (new Geometry)->thisJsval();
		}
		else if(1 == argc)
		{
			if(JSVAL_IS_OBJECT(argv[0]) && !JSVAL_IS_NULL(argv[0]))
			{
				JSObject *jso = JSVAL_TO_OBJECT(argv[0]);
				JsObject *obj = JsObject::thisObj(jso);
				if(!obj)
				{
					JS_ReportError(ecx()->_jsCx, "GeometryConstructor: arg must be an core object");
					return JS_FALSE;
				}

				Geometry *geometry = dynamic_cast<Geometry *>(obj);
				if(!geometry)
				{
					JS_ReportError(ecx()->_jsCx, "GeometryConstructor: arg must be a Geometry object");
					return JS_FALSE;
				}
				*rval = (new Geometry(*geometry))->thisJsval();
			}
			else
			{
				char *str;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &str)) return false;
				*rval = (new Geometry(str))->thisJsval();
			}

		}
		else if(2 == argc)
		{
			uint32 width, height;
			if(!JS_ConvertArguments(ecx()->_jsCx, argc, argv, "uu", &width, &height)) return false;
			*rval = (new Geometry(width, height))->thisJsval();
		}
		else if(4 == argc)
		{
			uint32 width, height;
			int32 xOff, yOff;
			if(!JS_ConvertArguments(ecx()->_jsCx, argc, argv, "uuii", &width, &height, &xOff, &yOff)) return false;
			*rval = (new Geometry(width, height, xOff, yOff))->thisJsval();
		}
		else if(6 == argc)
		{
			uint32 width, height;
			int32 xOff, yOff;
			JSBool xNegative, yNegative;
			if(!JS_ConvertArguments(ecx()->_jsCx, argc, argv, "uuiibb", &width, &height, &xOff, &yOff, &xNegative, &yNegative)) return false;
			*rval = (new Geometry(width, height, xOff, yOff, xNegative, yNegative))->thisJsval();
		}
		else 
		{
			JS_ReportError(ecx()->_jsCx, "GeometryConstructor: must be called with 1, 2, 4 or 6 args");
			return JS_FALSE;
		}

		return JS_TRUE;

	}

}}
