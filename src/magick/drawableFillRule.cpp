#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableFillRule.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableFillRule::DrawableFillRule(::Magick::FillRule from)
		: DrawableBase("DrawableFillRule")
		, _impl(new ::Magick::DrawableFillRule(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFillRule::DrawableFillRule(const DrawableFillRule &from)
		: DrawableBase("DrawableFillRule")
		, _impl(new ::Magick::DrawableFillRule(from.getImpl().fillRule()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFillRule::~DrawableFillRule()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFillRule::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableFillRule(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFillRule::xetter_fillRule(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->fillRule());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->fillRule((::Magick::FillRule)v);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableFillRule &DrawableFillRule::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFillRule::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableFillRule::call_copy, 0))return false;

		if(!jsRegisterProp("fillRule", &DrawableFillRule::xetter_fillRule, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableFillRuleConstructor::DrawableFillRuleConstructor()
		: JsObject(true, "DrawableFillRuleConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableFillRuleConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableFillRule *from = jsobjFromJsv<DrawableFillRule>(argv[0]);
			if(from)
			{
				*rval = (new DrawableFillRule(*from))->thisJsval();
			}
			else
			{
				uint32 v;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &v)) return false;
				*rval = (new DrawableFillRule((::Magick::FillRule)v))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "FillRuleConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
