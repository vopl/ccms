#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableStrokeWidth.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeWidth::DrawableStrokeWidth(double width_)
		: DrawableBase("DrawableStrokeWidth")
		, _impl(new ::Magick::DrawableStrokeWidth(width_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeWidth::DrawableStrokeWidth(const DrawableStrokeWidth &from)
		: DrawableBase("DrawableStrokeWidth")
		, _impl(new ::Magick::DrawableStrokeWidth(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeWidth::~DrawableStrokeWidth()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeWidth::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableStrokeWidth(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeWidth::xetter_width(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->width(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->width(d);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableStrokeWidth &DrawableStrokeWidth::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeWidth::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableStrokeWidth::call_copy, 0))return false;

		if(!jsRegisterProp("width", &DrawableStrokeWidth::xetter_width, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeWidthConstructor::DrawableStrokeWidthConstructor()
		: JsObject(true, "DrawableStrokeWidthConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableStrokeWidthConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableStrokeWidth *circle = jsobjFromJsv<DrawableStrokeWidth>(argv[0]);
			if(circle)
			{
				*rval = (new DrawableStrokeWidth(*circle))->thisJsval();
			}
			else
			{
				jsdouble width;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &width)) return false;

				*rval = (new DrawableStrokeWidth(width))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "StrokeWidthConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
