#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableStrokeOpacity.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeOpacity::DrawableStrokeOpacity(double opacity_)
		: DrawableBase("DrawableStrokeOpacity")
		, _impl(new ::Magick::DrawableStrokeOpacity(opacity_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeOpacity::DrawableStrokeOpacity(const DrawableStrokeOpacity &from)
		: DrawableBase("DrawableStrokeOpacity")
		, _impl(new ::Magick::DrawableStrokeOpacity(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeOpacity::~DrawableStrokeOpacity()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeOpacity::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableStrokeOpacity(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeOpacity::xetter_opacity(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->opacity(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->opacity(d);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableStrokeOpacity &DrawableStrokeOpacity::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeOpacity::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableStrokeOpacity::call_copy, 0))return false;

		if(!jsRegisterProp("opacity", &DrawableStrokeOpacity::xetter_opacity, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeOpacityConstructor::DrawableStrokeOpacityConstructor()
		: JsObject(true, "DrawableStrokeOpacityConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableStrokeOpacityConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableStrokeOpacity *circle = jsobjFromJsv<DrawableStrokeOpacity>(argv[0]);
			if(circle)
			{
				*rval = (new DrawableStrokeOpacity(*circle))->thisJsval();
			}
			else
			{
				jsdouble opacity;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &opacity)) return false;

				*rval = (new DrawableStrokeOpacity(opacity))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "StrokeOpacityConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
