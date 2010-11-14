#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableTranslation.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableTranslation::DrawableTranslation(double x_, double y_)
		: DrawableBase("DrawableTranslation")
		, _impl(new ::Magick::DrawableTranslation(x_, y_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableTranslation::DrawableTranslation(const DrawableTranslation &from)
		: DrawableBase("DrawableTranslation")
		, _impl(new ::Magick::DrawableTranslation(from.getImpl().x(), from.getImpl().y()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableTranslation::~DrawableTranslation()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTranslation::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableTranslation(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTranslation::xetter_x(jsval *vp, bool isGet)
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
	bool DrawableTranslation::xetter_y(jsval *vp, bool isGet)
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
	const ::Magick::DrawableTranslation &DrawableTranslation::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTranslation::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableTranslation::call_copy, 0))return false;

		if(!jsRegisterProp("x", &DrawableTranslation::xetter_x, false)) return false;
		if(!jsRegisterProp("y", &DrawableTranslation::xetter_y, false)) return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableTranslationConstructor::DrawableTranslationConstructor()
		: JsObject(true, "DrawableTranslationConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableTranslationConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableTranslation *from = jsobjFromJsv<DrawableTranslation>(argv[0]);
			if(from)
			{
				*rval = (new DrawableTranslation(*from))->thisJsval();
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "TranslationConstructor 1st arg must be a DrawableTranslation object");
				return false;
			}
		}
		else if(2 == argc)
		{
			jsdouble x, y;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &x, &y)) return false;
			*rval = (new DrawableTranslation(x, y))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "TranslationConstructor: must be called with 1 or 2 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
