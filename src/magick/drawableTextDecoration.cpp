#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableTextDecoration.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableTextDecoration::DrawableTextDecoration(::Magick::DecorationType from)
		: DrawableBase("DrawableTextDecoration")
		, _impl(new ::Magick::DrawableTextDecoration(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableTextDecoration::DrawableTextDecoration(const DrawableTextDecoration &from)
		: DrawableBase("DrawableTextDecoration")
		, _impl(new ::Magick::DrawableTextDecoration(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableTextDecoration::~DrawableTextDecoration()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTextDecoration::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableTextDecoration(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTextDecoration::xetter_decoration(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->decoration());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->decoration((::Magick::DecorationType)v);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableTextDecoration &DrawableTextDecoration::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTextDecoration::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableTextDecoration::call_copy, 0))return false;

		if(!jsRegisterProp("decoration", &DrawableTextDecoration::xetter_decoration, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableTextDecorationConstructor::DrawableTextDecorationConstructor()
		: JsObject(true, "DrawableTextDecorationConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableTextDecorationConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableTextDecoration *from = jsobjFromJsv<DrawableTextDecoration>(argv[0]);
			if(from)
			{
				*rval = (new DrawableTextDecoration(*from))->thisJsval();
			}
			else
			{
				uint32 v;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &v)) return false;
				*rval = (new DrawableTextDecoration((::Magick::DecorationType)v))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "DecorationConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
