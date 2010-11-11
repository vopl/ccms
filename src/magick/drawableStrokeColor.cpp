#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableStrokeColor.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeColor::DrawableStrokeColor()
		: DrawableBase("[object DrawableStrokeColor]")
		, _impl(new ::Magick::DrawableStrokeColor(::Magick::Color()))
	{
		(JSExceptionReporter)registerJs();
	}


	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeColor::DrawableStrokeColor(Color *from)
		: DrawableBase("[object DrawableStrokeColor]")
		, _impl(new ::Magick::DrawableStrokeColor(from->getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeColor::DrawableStrokeColor(const DrawableStrokeColor &from)
		: DrawableBase("[object DrawableStrokeColor]")
		, _impl(new ::Magick::DrawableStrokeColor(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeColor::~DrawableStrokeColor()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeColor::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableStrokeColor(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeColor::xetter_color(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = (new Color(_impl->color()))->thisJsval();
		}
		else
		{
			if(JSVAL_IS_OBJECT(*vp) && !JSVAL_IS_NULL(*vp))
			{
				JSObject *jso = JSVAL_TO_OBJECT(*vp);
				JsObject *obj = JsObject::thisObj(jso);
				if(!obj)
				{
					JS_ReportError(ecx()->_jsCx, "DrawableStrokeColor.color: arg must be an core object");
					return JS_FALSE;
				}

				Color *color = dynamic_cast<Color *>(obj);
				if(!color)
				{
					JS_ReportError(ecx()->_jsCx, "DrawableStrokeColor.color: arg must be a Color object");
					return JS_FALSE;
				}
				_impl->color(color->getImpl());
			}
			else
			{
				char *strValue;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &strValue)) return false;
				_impl->color(::Magick::Color(strValue));
			}
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableStrokeColor &DrawableStrokeColor::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeColor::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableStrokeColor::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeColorConstructor::DrawableStrokeColorConstructor()
		: JsObject(true, "[object DrawableStrokeColorConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableStrokeColorConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableStrokeColor *from = jsobjFromJsv<DrawableStrokeColor>(argv[0]);
			if(from)
			{
				*rval = (new DrawableStrokeColor(*from))->thisJsval();
			}
			else
			{
				DrawableStrokeColor *res = new DrawableStrokeColor();
				if(!res->xetter_color(argv, false)) return false;
				*rval = res->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "StrokeColorConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
