#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawablePushGraphicContext.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePushGraphicContext::DrawablePushGraphicContext()
		: DrawableBase("DrawablePushGraphicContext")
		, _impl(new ::Magick::DrawablePushGraphicContext)
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePushGraphicContext::~DrawablePushGraphicContext()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePushGraphicContext::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePushGraphicContext(*this))->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePushGraphicContext &DrawablePushGraphicContext::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePushGraphicContext::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePushGraphicContext::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePushGraphicContextConstructor::DrawablePushGraphicContextConstructor()
		: JsObject(true, "DrawablePushGraphicContextConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePushGraphicContextConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(0 == argc)
		{
			*rval = (new DrawablePushGraphicContext)->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PushGraphicContextConstructor: must be called with 0 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
