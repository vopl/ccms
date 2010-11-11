#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableRotation.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableRotation::DrawableRotation(double angle_)
		: DrawableBase("[object DrawableRotation]")
		, _impl(new ::Magick::DrawableRotation(angle_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableRotation::DrawableRotation(const DrawableRotation &from)
		: DrawableBase("[object DrawableRotation]")
		, _impl(new ::Magick::DrawableRotation(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableRotation::~DrawableRotation()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRotation::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableRotation(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRotation::xetter_angle(jsval *vp, bool isGet)
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
	const ::Magick::DrawableRotation &DrawableRotation::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRotation::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableRotation::call_copy, 0))return false;

		if(!jsRegisterProp("angle", &DrawableRotation::xetter_angle, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableRotationConstructor::DrawableRotationConstructor()
		: JsObject(true, "[object DrawableRotationConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableRotationConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableRotation *circle = jsobjFromJsv<DrawableRotation>(argv[0]);
			if(circle)
			{
				*rval = (new DrawableRotation(*circle))->thisJsval();
			}
			else
			{
				jsdouble angle;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &angle)) return false;

				*rval = (new DrawableRotation(angle))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "RotationConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
