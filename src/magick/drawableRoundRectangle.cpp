#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableRoundRectangle.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableRoundRectangle::DrawableRoundRectangle(double centerX_, double centerY_, double width_, double hight_, double cornerWidth_, double cornerHight_)
		: DrawableBase("[object DrawableRoundRectangle]")
		, _impl(new ::Magick::DrawableRoundRectangle(centerX_, centerY_, width_, hight_, cornerWidth_, cornerHight_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableRoundRectangle::DrawableRoundRectangle(const DrawableRoundRectangle &from)
		: DrawableBase("[object DrawableRoundRectangle]")
		, _impl(new ::Magick::DrawableRoundRectangle(from.getImpl().centerX(), from.getImpl().centerY(), from.getImpl().width(), from.getImpl().hight(), from.getImpl().cornerWidth(), from.getImpl().cornerHeight()))
	{

	}

	//////////////////////////////////////////////////////////////////////////
	DrawableRoundRectangle::~DrawableRoundRectangle()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRoundRectangle::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableRoundRectangle(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRoundRectangle::xetter_centerX(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->centerX(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->centerX(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRoundRectangle::xetter_centerY(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->centerY(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->centerY(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRoundRectangle::xetter_width(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->width(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->width(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRoundRectangle::xetter_height(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->hight(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->hight(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRoundRectangle::xetter_cornerWidth(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->cornerWidth(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->cornerWidth(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRoundRectangle::xetter_cornerHeight(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->cornerHeight(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->cornerHeight(d);
		}

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableRoundRectangle &DrawableRoundRectangle::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRoundRectangle::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableRoundRectangle::call_copy, 0))return false;

		if(!jsRegisterProp("centerX", &DrawableRoundRectangle::xetter_centerX, false)) return false;
		if(!jsRegisterProp("centerY", &DrawableRoundRectangle::xetter_centerY, false)) return false;
		if(!jsRegisterProp("width", &DrawableRoundRectangle::xetter_width, false)) return false;
		if(!jsRegisterProp("height", &DrawableRoundRectangle::xetter_height, false)) return false;
		if(!jsRegisterProp("cornerWidth", &DrawableRoundRectangle::xetter_cornerWidth, false)) return false;
		if(!jsRegisterProp("cornerHeight", &DrawableRoundRectangle::xetter_cornerHeight, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableRoundRectangleConstructor::DrawableRoundRectangleConstructor()
		: JsObject(true, "[object DrawableRoundRectangleConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableRoundRectangleConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableRoundRectangle *circle = jsobjFromJsv<DrawableRoundRectangle>(argv[0]);
			if(!circle)
			{
				JS_ReportError(ecx()->_jsCx, "RoundRectangleConstructor: arg must be a DrawableRoundRectangle object");
				return JS_FALSE;
			}
			*rval = (new DrawableRoundRectangle(*circle))->thisJsval();
		}
		else if(6 == argc)
		{
			jsdouble centerX_, centerY_, width_, hight_, cornerWidth_, cornerHight_;
			if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "dddd", &centerX_, &centerY_, &width_, &hight_, &cornerWidth_, &cornerHight_)) return false;

			*rval = (new DrawableRoundRectangle(centerX_, centerY_, width_, hight_, cornerWidth_, cornerHight_))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "RoundRectangleConstructor: must be called with 1 or 6 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
