#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableSkewX.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableSkewX::DrawableSkewX(double angle_)
		: DrawableBase("[object DrawableSkewX]")
		, _impl(new ::Magick::DrawableSkewX(angle_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableSkewX::DrawableSkewX(const DrawableSkewX &from)
		: DrawableBase("[object DrawableSkewX]")
		, _impl(new ::Magick::DrawableSkewX(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableSkewX::~DrawableSkewX()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableSkewX::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableSkewX(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableSkewX::xetter_angle(jsval *vp, bool isGet)
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
	const ::Magick::DrawableSkewX &DrawableSkewX::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableSkewX::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableSkewX::call_copy, 0))return false;

		if(!jsRegisterProp("angle", &DrawableSkewX::xetter_angle, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableSkewXConstructor::DrawableSkewXConstructor()
		: JsObject(true, "[object DrawableSkewXConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableSkewXConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableSkewX *circle = jsobjFromJsv<DrawableSkewX>(argv[0]);
			if(circle)
			{
				*rval = (new DrawableSkewX(*circle))->thisJsval();
			}
			else
			{
				jsdouble angle;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &angle)) return false;

				*rval = (new DrawableSkewX(angle))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "SkewXConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
