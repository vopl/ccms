#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableText.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableText::DrawableText(double x_, double y_, const char *text)
		: DrawableBase("DrawableText")
		, _impl(new ::Magick::DrawableText(x_, y_, text))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableText::DrawableText(const DrawableText &from)
		: DrawableBase("DrawableText")
		, _impl(new ::Magick::DrawableText(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableText::~DrawableText()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableText::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableText(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableText::xetter_x(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->x(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->x(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableText::xetter_y(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->y(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->y(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableText::xetter_text(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			std::string str = _impl->text();
			*vp = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, str.data(), str.size()));
		}
		else
		{
			char *str;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &str)) return false;
			_impl->text(str);
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableText &DrawableText::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableText::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableText::call_copy, 0))return false;

		if(!jsRegisterProp("x", &DrawableText::xetter_x, false)) return false;
		if(!jsRegisterProp("y", &DrawableText::xetter_y, false)) return false;
		if(!jsRegisterProp("text", &DrawableText::xetter_text, false)) return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableTextConstructor::DrawableTextConstructor()
		: JsObject(true, "DrawableTextConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableTextConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableText *from = jsobjFromJsv<DrawableText>(argv[0]);
			if(from)
			{
				*rval = (new DrawableText(*from))->thisJsval();
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "TextConstructor 1st arg must be a DrawableText object");
				return false;
			}
		}
		else if(3 == argc)
		{
			jsdouble x, y;
			char *text;
			if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "dds", &x, &y, &text)) return false;
			*rval = (new DrawableText(x, y, text))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "TextConstructor: must be called with 1 or 3 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
