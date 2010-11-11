#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableScaling.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableScaling::DrawableScaling(double x_, double y_)
		: DrawableBase("[object DrawableScaling]")
		, _impl(new ::Magick::DrawableScaling(x_, y_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableScaling::DrawableScaling(const DrawableScaling &from)
		: DrawableBase("[object DrawableScaling]")
		, _impl(new ::Magick::DrawableScaling(from.getImpl().x(), from.getImpl().y()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableScaling::~DrawableScaling()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableScaling::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableScaling(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableScaling::xetter_x(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->x(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->x(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableScaling::xetter_y(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->y(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->y(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableScaling &DrawableScaling::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableScaling::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableScaling::call_copy, 0))return false;

		if(!jsRegisterProp("x", &DrawableScaling::xetter_x, false)) return false;
		if(!jsRegisterProp("y", &DrawableScaling::xetter_y, false)) return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableScalingConstructor::DrawableScalingConstructor()
		: JsObject(true, "[object DrawableScalingConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableScalingConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableScaling *from = jsobjFromJsv<DrawableScaling>(argv[0]);
			if(from)
			{
				*rval = (new DrawableScaling(*from))->thisJsval();
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "ScalingConstructor 1st arg must be a DrawableScaling object");
				return false;
			}
		}
		else if(2 == argc)
		{
			jsdouble x, y;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &x, &y)) return false;
			*rval = (new DrawableScaling(x, y))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "ScalingConstructor: must be called with 1 or 2 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
