#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableStrokeAntialias.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeAntialias::DrawableStrokeAntialias(bool flag_)
		: DrawableBase("DrawableStrokeAntialias")
		, _impl(new ::Magick::DrawableStrokeAntialias(flag_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeAntialias::DrawableStrokeAntialias(const DrawableStrokeAntialias &from)
		: DrawableBase("DrawableStrokeAntialias")
		, _impl(new ::Magick::DrawableStrokeAntialias(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeAntialias::~DrawableStrokeAntialias()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeAntialias::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableStrokeAntialias(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeAntialias::xetter_flag(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->flag(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->flag(d);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableStrokeAntialias &DrawableStrokeAntialias::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeAntialias::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableStrokeAntialias::call_copy, 0))return false;

		if(!jsRegisterProp("flag", &DrawableStrokeAntialias::xetter_flag, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeAntialiasConstructor::DrawableStrokeAntialiasConstructor()
		: JsObject(true, "DrawableStrokeAntialiasConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableStrokeAntialiasConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableStrokeAntialias *circle = jsobjFromJsv<DrawableStrokeAntialias>(argv[0]);
			if(circle)
			{
				*rval = (new DrawableStrokeAntialias(*circle))->thisJsval();
			}
			else
			{
				bool flag;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &flag)) return false;

				*rval = (new DrawableStrokeAntialias(flag))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "StrokeAntialiasConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
