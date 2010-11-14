#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableSkewY.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableSkewY::DrawableSkewY(double angle_)
		: DrawableBase("DrawableSkewY")
		, _impl(new ::Magick::DrawableSkewY(angle_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableSkewY::DrawableSkewY(const DrawableSkewY &from)
		: DrawableBase("DrawableSkewY")
		, _impl(new ::Magick::DrawableSkewY(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableSkewY::~DrawableSkewY()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableSkewY::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableSkewY(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableSkewY::xetter_angle(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->angle(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->angle(d);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableSkewY &DrawableSkewY::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableSkewY::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableSkewY::call_copy, 0))return false;

		if(!jsRegisterProp("angle", &DrawableSkewY::xetter_angle, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableSkewYConstructor::DrawableSkewYConstructor()
		: JsObject(true, "DrawableSkewYConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableSkewYConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableSkewY *circle = jsobjFromJsv<DrawableSkewY>(argv[0]);
			if(circle)
			{
				*rval = (new DrawableSkewY(*circle))->thisJsval();
			}
			else
			{
				jsdouble angle;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &angle)) return false;

				*rval = (new DrawableSkewY(angle))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "SkewYConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
