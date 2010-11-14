#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawablePoint.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePoint::DrawablePoint(double x_, double y_)
		: DrawableBase("DrawablePoint")
		, _impl(new ::Magick::DrawablePoint(x_, y_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePoint::DrawablePoint(const DrawablePoint &from)
		: DrawableBase("DrawablePoint")
		, _impl(new ::Magick::DrawablePoint(from.getImpl().x(), from.getImpl().y()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePoint::~DrawablePoint()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePoint::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePoint(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePoint::xetter_x(jsval *vp, bool isGet)
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
	bool DrawablePoint::xetter_y(jsval *vp, bool isGet)
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
	const ::Magick::DrawablePoint &DrawablePoint::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePoint::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePoint::call_copy, 0))return false;

		if(!jsRegisterProp("x", &DrawablePoint::xetter_x, false)) return false;
		if(!jsRegisterProp("y", &DrawablePoint::xetter_y, false)) return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePointConstructor::DrawablePointConstructor()
		: JsObject(true, "DrawablePointConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePointConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawablePoint *from = jsobjFromJsv<DrawablePoint>(argv[0]);
			if(from)
			{
				*rval = (new DrawablePoint(*from))->thisJsval();
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "PointConstructor 1st arg must be a DrawablePoint object");
				return false;
			}
		}
		else if(2 == argc)
		{
			jsdouble x, y;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &x, &y)) return false;
			*rval = (new DrawablePoint(x, y))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PointConstructor: must be called with 1 or 2 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
