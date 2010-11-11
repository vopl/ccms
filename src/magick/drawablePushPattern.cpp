#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawablePushPattern.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePushPattern::DrawablePushPattern(const char *id_, int32 x_, int32 y_, uint32 width_, uint32 height_)
		: DrawableBase("[object DrawablePushPattern]")
		, _impl(new ::Magick::DrawablePushPattern(id_, x_, y_, width_, height_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePushPattern::DrawablePushPattern(const DrawablePushPattern &from)
		: DrawableBase("[object DrawablePushPattern]")
		, _impl(new ::Magick::DrawablePushPattern(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePushPattern::~DrawablePushPattern()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePushPattern::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePushPattern(*this))->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePushPattern &DrawablePushPattern::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePushPattern::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePushPattern::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePushPatternConstructor::DrawablePushPatternConstructor()
		: JsObject(true, "[object DrawablePushPatternConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePushPatternConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawablePushPattern *from = jsobjFromJsv<DrawablePushPattern>(argv[0]);
			if(from)
			{
				*rval = (new DrawablePushPattern(*from))->thisJsval();
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "PushPatternConstructor: 1st arg must be must be DrawablePushPattern object");
				return JS_FALSE;
			}
		}
		else if(5 == argc)
		{
			const char *id_;
			int32 x_;
			int32 y_;
			uint32 width_;
			uint32 height_;
			if(!JS_ConvertArguments(ecx()->_jsCx, 5, argv, "siiuu", &id_, &x_, &y_, &width_, &height_)) return false;

			*rval = (new DrawablePushPattern(id_, x_, y_, width_, height_))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PushPatternConstructor: must be called with 1 or 5 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
