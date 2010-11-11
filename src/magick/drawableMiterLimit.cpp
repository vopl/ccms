#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableMiterLimit.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableMiterLimit::DrawableMiterLimit(uint32 from)
		: DrawableBase("[object DrawableMiterLimit]")
		, _impl(new ::Magick::DrawableMiterLimit(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableMiterLimit::DrawableMiterLimit(const DrawableMiterLimit &from)
		: DrawableBase("[object DrawableMiterLimit]")
		, _impl(new ::Magick::DrawableMiterLimit(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableMiterLimit::~DrawableMiterLimit()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableMiterLimit::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableMiterLimit(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableMiterLimit::xetter_miterLimit(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->miterlimit());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->miterlimit(v);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableMiterLimit &DrawableMiterLimit::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableMiterLimit::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableMiterLimit::call_copy, 0))return false;

		if(!jsRegisterProp("miterLimit", &DrawableMiterLimit::xetter_miterLimit, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableMiterLimitConstructor::DrawableMiterLimitConstructor()
		: JsObject(true, "[object DrawableMiterLimitConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableMiterLimitConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableMiterLimit *from = jsobjFromJsv<DrawableMiterLimit>(argv[0]);
			if(from)
			{
				*rval = (new DrawableMiterLimit(*from))->thisJsval();
			}
			else
			{
				uint32 v;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &v)) return false;
				*rval = (new DrawableMiterLimit(v))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "MiterLimitConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
