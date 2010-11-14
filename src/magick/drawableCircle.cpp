#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableCircle.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableCircle::DrawableCircle(double originX_, double originY_, double perimX_, double perimY_)
		: DrawableBase("DrawableCircle")
		, _impl(new ::Magick::DrawableCircle(originX_, originY_, perimX_, perimY_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableCircle::DrawableCircle(const DrawableCircle &from)
		: DrawableBase("DrawableCircle")
		, _impl(new ::Magick::DrawableCircle(from.getImpl().originX(), from.getImpl().originY(), from.getImpl().perimX(), from.getImpl().perimY()))
	{

	}

	//////////////////////////////////////////////////////////////////////////
	DrawableCircle::~DrawableCircle()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCircle::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableCircle(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCircle::xetter_originX(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->originX(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->originX(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCircle::xetter_originY(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->originY(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->originY(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCircle::xetter_perimX(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->perimX(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->perimX(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCircle::xetter_perimY(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->perimY(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->perimY(d);
		}

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableCircle &DrawableCircle::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCircle::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableCircle::call_copy, 0))return false;

		if(!jsRegisterProp("originX", &DrawableCircle::xetter_originX, false)) return false;
		if(!jsRegisterProp("originY", &DrawableCircle::xetter_originY, false)) return false;
		if(!jsRegisterProp("perimX", &DrawableCircle::xetter_perimX, false)) return false;
		if(!jsRegisterProp("perimY", &DrawableCircle::xetter_perimY, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableCircleConstructor::DrawableCircleConstructor()
		: JsObject(true, "DrawableCircleConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableCircleConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableCircle *circle = jsobjFromJsv<DrawableCircle>(argv[0]);
			if(!circle)
			{
				JS_ReportError(ecx()->_jsCx, "CircleConstructor: arg must be a DrawableCircle object");
				return JS_FALSE;
			}
			*rval = (new DrawableCircle(*circle))->thisJsval();
		}
		else if(4 == argc)
		{
			jsdouble originX_, originY_, perimX_, perimY_;
			if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "dddd", &originX_, &originY_, &perimX_, &perimY_)) return false;

			*rval = (new DrawableCircle(originX_, originY_, perimX_, perimY_))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "CircleConstructor: must be called with 1 or 4 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
