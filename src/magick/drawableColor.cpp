#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableColor.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableColor::DrawableColor(double x_, double y_, ::Magick::PaintMethod paintMethod_)
		: DrawableBase("DrawableColor")
		, _impl(new ::Magick::DrawableColor(x_, y_, paintMethod_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableColor::DrawableColor(const DrawableColor &from)
		: DrawableBase("DrawableColor")
		, _impl(new ::Magick::DrawableColor(from.getImpl().x(), from.getImpl().y(), from.getImpl().paintMethod()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableColor::~DrawableColor()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableColor::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableColor(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableColor::xetter_x(jsval *vp, bool isGet)
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
	bool DrawableColor::xetter_y(jsval *vp, bool isGet)
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

	bool DrawableColor::xetter_paintMethod(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->paintMethod());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->paintMethod((::Magick::PaintMethod)v);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableColor &DrawableColor::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableColor::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableColor::call_copy, 0))return false;

		if(!jsRegisterProp("x", &DrawableColor::xetter_x, false)) return false;
		if(!jsRegisterProp("y", &DrawableColor::xetter_y, false)) return false;
		if(!jsRegisterProp("paintMethod", &DrawableColor::xetter_paintMethod, false)) return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableColorConstructor::DrawableColorConstructor()
		: JsObject(true, "DrawableColorConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableColorConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableColor *from = jsobjFromJsv<DrawableColor>(argv[0]);
			if(from)
			{
				*rval = (new DrawableColor(*from))->thisJsval();
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "ColorConstructor 1st arg must be a DrawableColor object");
				return false;
			}
		}
		else if(3 == argc)
		{
			jsdouble x, y;
			uint32 paintMethod;

			if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "ddu", &x, &y, &paintMethod)) return false;
			*rval = (new DrawableColor(x, y, (::Magick::PaintMethod)paintMethod))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "ColorConstructor: must be called with 1 or 3 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
