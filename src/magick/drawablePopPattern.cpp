#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawablePopPattern.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePopPattern::DrawablePopPattern()
		: DrawableBase("DrawablePopPattern")
		, _impl(new ::Magick::DrawablePopPattern())
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePopPattern::~DrawablePopPattern()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePopPattern::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePopPattern(*this))->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePopPattern &DrawablePopPattern::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePopPattern::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePopPattern::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePopPatternConstructor::DrawablePopPatternConstructor()
		: JsObject(true, "DrawablePopPatternConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePopPatternConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(0 == argc)
		{
			*rval = (new DrawablePopPattern)->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PopPatternConstructor: must be called with 0 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
