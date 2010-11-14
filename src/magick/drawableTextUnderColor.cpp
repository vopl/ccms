#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableTextUnderColor.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableTextUnderColor::DrawableTextUnderColor(Color *from)
		: DrawableBase("DrawableTextUnderColor")
		, _impl(new ::Magick::DrawableTextUnderColor(from->getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableTextUnderColor::DrawableTextUnderColor(const DrawableTextUnderColor &from)
		: DrawableBase("DrawableTextUnderColor")
		, _impl(new ::Magick::DrawableTextUnderColor(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableTextUnderColor::~DrawableTextUnderColor()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTextUnderColor::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableTextUnderColor(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTextUnderColor::xetter_color(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = (new Color(_impl->color()))->thisJsval();
		}
		else
		{
			if(JSVAL_IS_OBJECT(*vp) && !JSVAL_IS_NULL(*vp))
			{
				JSObject *jso = JSVAL_TO_OBJECT(*vp);
				JsObject *obj = JsObject::thisObj(jso);
				if(!obj)
				{
					JS_ReportError(ecx()->_jsCx, "DrawableTextUnderColor.color: arg must be an core object");
					return JS_FALSE;
				}

				Color *color = dynamic_cast<Color *>(obj);
				if(!color)
				{
					JS_ReportError(ecx()->_jsCx, "DrawableTextUnderColor.color: arg must be a Color object");
					return JS_FALSE;
				}
				_impl->color(color->getImpl());
			}
			else
			{
				char *strValue;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &strValue)) return false;
				_impl->color(::Magick::Color(strValue));
			}
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableTextUnderColor &DrawableTextUnderColor::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableTextUnderColor::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableTextUnderColor::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableTextUnderColorConstructor::DrawableTextUnderColorConstructor()
		: JsObject(true, "DrawableTextUnderColorConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableTextUnderColorConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableTextUnderColor *from = jsobjFromJsv<DrawableTextUnderColor>(argv[0]);
			if(from)
			{
				*rval = (new DrawableTextUnderColor(*from))->thisJsval();
			}
			else
			{
				DrawableTextUnderColor *res = new DrawableTextUnderColor(*from);
				if(!res->xetter_color(argv, false)) return false;
				*rval = res->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "TextUnderColorConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
