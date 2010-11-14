#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableTextAntialias.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableTextAntialias::DrawableTextAntialias(bool flag_)
		: DrawableBase("DrawableTextAntialias")
		, _impl(new ::Magick::DrawableTextAntialias(flag_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableTextAntialias::DrawableTextAntialias(const DrawableTextAntialias &from)
		: DrawableBase("DrawableTextAntialias")
		, _impl(new ::Magick::DrawableTextAntialias(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableTextAntialias::~DrawableTextAntialias()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTextAntialias::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableTextAntialias(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTextAntialias::xetter_flag(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->flag(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->flag(d);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableTextAntialias &DrawableTextAntialias::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTextAntialias::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableTextAntialias::call_copy, 0))return false;

		if(!jsRegisterProp("flag", &DrawableTextAntialias::xetter_flag, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableTextAntialiasConstructor::DrawableTextAntialiasConstructor()
		: JsObject(true, "DrawableTextAntialiasConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableTextAntialiasConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableTextAntialias *circle = jsobjFromJsv<DrawableTextAntialias>(argv[0]);
			if(circle)
			{
				*rval = (new DrawableTextAntialias(*circle))->thisJsval();
			}
			else
			{
				bool flag;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &flag)) return false;

				*rval = (new DrawableTextAntialias(flag))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "TextAntialiasConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
