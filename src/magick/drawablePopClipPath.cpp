#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawablePopClipPath.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePopClipPath::DrawablePopClipPath()
		: DrawableBase("[object DrawablePopClipPath]")
		, _impl(new ::Magick::DrawablePopClipPath())
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePopClipPath::~DrawablePopClipPath()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePopClipPath::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePopClipPath(*this))->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePopClipPath &DrawablePopClipPath::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePopClipPath::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePopClipPath::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePopClipPathConstructor::DrawablePopClipPathConstructor()
		: JsObject(true, "[object DrawablePopClipPathConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePopClipPathConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(0 == argc)
		{
			*rval = (new DrawablePopClipPath)->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PopClipPathConstructor: must be called with 0 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
