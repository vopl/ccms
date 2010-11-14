#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableRectangle.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableRectangle::DrawableRectangle(double upperLeftX_, double upperLeftY_, double lowerRightX_, double lowerRightY_)
		: DrawableBase("DrawableRectangle")
		, _impl(new ::Magick::DrawableRectangle(upperLeftX_, upperLeftY_, lowerRightX_, lowerRightY_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableRectangle::DrawableRectangle(const DrawableRectangle &from)
		: DrawableBase("DrawableRectangle")
		, _impl(new ::Magick::DrawableRectangle(from.getImpl().upperLeftX(), from.getImpl().upperLeftY(), from.getImpl().lowerRightX(), from.getImpl().lowerRightY()))
	{

	}

	//////////////////////////////////////////////////////////////////////////
	DrawableRectangle::~DrawableRectangle()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRectangle::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableRectangle(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRectangle::xetter_upperLeftX(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->upperLeftX(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->upperLeftX(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRectangle::xetter_upperLeftY(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->upperLeftY(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->upperLeftY(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRectangle::xetter_lowerRightX(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->lowerRightX(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->lowerRightX(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRectangle::xetter_lowerRightY(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->lowerRightY(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->lowerRightY(d);
		}

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableRectangle &DrawableRectangle::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableRectangle::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableRectangle::call_copy, 0))return false;

		if(!jsRegisterProp("upperLeftX", &DrawableRectangle::xetter_upperLeftX, false)) return false;
		if(!jsRegisterProp("upperLeftY", &DrawableRectangle::xetter_upperLeftY, false)) return false;
		if(!jsRegisterProp("lowerRightX", &DrawableRectangle::xetter_lowerRightX, false)) return false;
		if(!jsRegisterProp("lowerRightY", &DrawableRectangle::xetter_lowerRightY, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableRectangleConstructor::DrawableRectangleConstructor()
		: JsObject(true, "DrawableRectangleConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableRectangleConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableRectangle *circle = jsobjFromJsv<DrawableRectangle>(argv[0]);
			if(!circle)
			{
				JS_ReportError(ecx()->_jsCx, "RectangleConstructor: arg must be a DrawableRectangle object");
				return JS_FALSE;
			}
			*rval = (new DrawableRectangle(*circle))->thisJsval();
		}
		else if(4 == argc)
		{
			jsdouble upperLeftX_, upperLeftY_, lowerRightX_, lowerRightY_;
			if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "dddd", &upperLeftX_, &upperLeftY_, &lowerRightX_, &lowerRightY_)) return false;

			*rval = (new DrawableRectangle(upperLeftX_, upperLeftY_, lowerRightX_, lowerRightY_))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "RectangleConstructor: must be called with 1 or 4 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
