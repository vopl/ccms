#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableStrokeLineCap.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeLineCap::DrawableStrokeLineCap(::Magick::LineCap from)
		: DrawableBase("[object DrawableStrokeLineCap]")
		, _impl(new ::Magick::DrawableStrokeLineCap(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeLineCap::DrawableStrokeLineCap(const DrawableStrokeLineCap &from)
		: DrawableBase("[object DrawableStrokeLineCap]")
		, _impl(new ::Magick::DrawableStrokeLineCap(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeLineCap::~DrawableStrokeLineCap()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeLineCap::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableStrokeLineCap(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeLineCap::xetter_lineCap(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->linecap());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->linecap((::Magick::LineCap)v);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableStrokeLineCap &DrawableStrokeLineCap::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeLineCap::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableStrokeLineCap::call_copy, 0))return false;

		if(!jsRegisterProp("lineCap", &DrawableStrokeLineCap::xetter_lineCap, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeLineCapConstructor::DrawableStrokeLineCapConstructor()
		: JsObject(true, "[object DrawableStrokeLineCapConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableStrokeLineCapConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableStrokeLineCap *from = jsobjFromJsv<DrawableStrokeLineCap>(argv[0]);
			if(from)
			{
				*rval = (new DrawableStrokeLineCap(*from))->thisJsval();
			}
			else
			{
				uint32 v;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &v)) return false;
				*rval = (new DrawableStrokeLineCap((::Magick::LineCap)v))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "StrokeLineCapConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
