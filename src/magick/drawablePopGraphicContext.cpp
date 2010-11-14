#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawablePopGraphicContext.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePopGraphicContext::DrawablePopGraphicContext()
		: DrawableBase("DrawablePopGraphicContext")
		, _impl(new ::Magick::DrawablePopGraphicContext())
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePopGraphicContext::~DrawablePopGraphicContext()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePopGraphicContext::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePopGraphicContext(*this))->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePopGraphicContext &DrawablePopGraphicContext::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePopGraphicContext::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePopGraphicContext::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePopGraphicContextConstructor::DrawablePopGraphicContextConstructor()
		: JsObject(true, "DrawablePopGraphicContextConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePopGraphicContextConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(0 == argc)
		{
			*rval = (new DrawablePopGraphicContext)->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PopGraphicContextConstructor: must be called with 0 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
