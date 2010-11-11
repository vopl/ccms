#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableDashOffset.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableDashOffset::DrawableDashOffset(const double offset_)
		: DrawableBase("[object DrawableDashOffset]")
		, _impl(new ::Magick::DrawableDashOffset(offset_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableDashOffset::DrawableDashOffset(const DrawableDashOffset &from)
		: DrawableBase("[object DrawableDashOffset]")
		, _impl(new ::Magick::DrawableDashOffset(from.getImpl().offset()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableDashOffset::~DrawableDashOffset()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableDashOffset::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableDashOffset(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableDashOffset::xetter_offset(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->offset(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->offset(d);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableDashOffset &DrawableDashOffset::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableDashOffset::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableDashOffset::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableDashOffsetConstructor::DrawableDashOffsetConstructor()
		: JsObject(true, "[object DrawableDashOffsetConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableDashOffsetConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableDashOffset *from = jsobjFromJsv<DrawableDashOffset>(argv[0]);
			if(from)
			{
				*rval = (new DrawableDashOffset(*from))->thisJsval();
			}
			else
			{
				jsdouble v;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &v)) return false;
				*rval = (new DrawableDashOffset(v))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "DashOffsetConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
