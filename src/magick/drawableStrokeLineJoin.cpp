#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableStrokeLineJoin.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeLineJoin::DrawableStrokeLineJoin(::Magick::LineJoin from)
		: DrawableBase("[object DrawableStrokeLineJoin]")
		, _impl(new ::Magick::DrawableStrokeLineJoin(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeLineJoin::DrawableStrokeLineJoin(const DrawableStrokeLineJoin &from)
		: DrawableBase("[object DrawableStrokeLineJoin]")
		, _impl(new ::Magick::DrawableStrokeLineJoin(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeLineJoin::~DrawableStrokeLineJoin()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeLineJoin::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableStrokeLineJoin(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeLineJoin::xetter_lineJoin(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->linejoin());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->linejoin((::Magick::LineJoin)v);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableStrokeLineJoin &DrawableStrokeLineJoin::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableStrokeLineJoin::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableStrokeLineJoin::call_copy, 0))return false;

		if(!jsRegisterProp("lineJoin", &DrawableStrokeLineJoin::xetter_lineJoin, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableStrokeLineJoinConstructor::DrawableStrokeLineJoinConstructor()
		: JsObject(true, "[object DrawableStrokeLineJoinConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableStrokeLineJoinConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableStrokeLineJoin *from = jsobjFromJsv<DrawableStrokeLineJoin>(argv[0]);
			if(from)
			{
				*rval = (new DrawableStrokeLineJoin(*from))->thisJsval();
			}
			else
			{
				uint32 v;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &v)) return false;
				*rval = (new DrawableStrokeLineJoin((::Magick::LineJoin)v))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "StrokeLineJoinConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
