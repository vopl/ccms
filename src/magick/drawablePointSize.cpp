#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawablePointSize.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePointSize::DrawablePointSize(double pointSize_)
		: DrawableBase("[object DrawablePointSize]")
		, _impl(new ::Magick::DrawablePointSize(pointSize_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePointSize::DrawablePointSize(const DrawablePointSize &from)
		: DrawableBase("[object DrawablePointSize]")
		, _impl(new ::Magick::DrawablePointSize(from.getImpl().pointSize()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePointSize::~DrawablePointSize()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePointSize::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePointSize(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePointSize::xetter_pointSize(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->pointSize(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->pointSize(d);
		}

		return true;
	}

	
	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePointSize &DrawablePointSize::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePointSize::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePointSize::call_copy, 0))return false;

		if(!jsRegisterProp("pointSize", &DrawablePointSize::xetter_pointSize, false)) return false;
		

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePointSizeConstructor::DrawablePointSizeConstructor()
		: JsObject(true, "[object DrawablePointSizeConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePointSizeConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawablePointSize *from = jsobjFromJsv<DrawablePointSize>(argv[0]);
			if(from)
			{
				*rval = (new DrawablePointSize(*from))->thisJsval();
			}
			else
			{
				jsdouble pointSize;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &pointSize)) return false;
				*rval = (new DrawablePointSize(pointSize))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PointSizeConstructor: must be called with 1 or 3 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
