#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableFillOpacity.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableFillOpacity::DrawableFillOpacity(double opacity_)
		: DrawableBase("DrawableFillOpacity")
		, _impl(new ::Magick::DrawableFillOpacity(opacity_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFillOpacity::DrawableFillOpacity(const DrawableFillOpacity &from)
		: DrawableBase("DrawableFillOpacity")
		, _impl(new ::Magick::DrawableFillOpacity(from.getImpl().opacity()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFillOpacity::~DrawableFillOpacity()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFillOpacity::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableFillOpacity(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFillOpacity::xetter_opacity(jsval *vp, bool isGet)
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
	const ::Magick::DrawableFillOpacity &DrawableFillOpacity::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFillOpacity::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableFillOpacity::call_copy, 0))return false;

		if(!jsRegisterProp("opacity", &DrawableFillOpacity::xetter_opacity, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableFillOpacityConstructor::DrawableFillOpacityConstructor()
		: JsObject(true, "DrawableFillOpacityConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableFillOpacityConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableFillOpacity *from = jsobjFromJsv<DrawableFillOpacity>(argv[0]);
			if(from)
			{
				*rval = (new DrawableFillOpacity(*from))->thisJsval();
			}
			else
			{
				jsdouble opacity_;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &opacity_)) return false;

				*rval = (new DrawableFillOpacity(opacity_))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "FillOpacityConstructor: must be called with 1 or 6 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
