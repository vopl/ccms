#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableFillColor.hpp"

namespace ccms {namespace magick{

	DrawableFillColor::DrawableFillColor()
		: DrawableBase("DrawableStrokeColor")
		, _impl(new ::Magick::DrawableFillColor(::Magick::Color()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFillColor::DrawableFillColor(Color *from)
		: DrawableBase("DrawableFillColor")
		, _impl(new ::Magick::DrawableFillColor(from->getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFillColor::DrawableFillColor(const DrawableFillColor &from)
		: DrawableBase("DrawableFillColor")
		, _impl(new ::Magick::DrawableFillColor(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFillColor::~DrawableFillColor()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFillColor::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableFillColor(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFillColor::xetter_color(jsval *vp, bool isGet)
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
					JS_ReportError(ecx()->_jsCx, "DrawableFillColor.color: arg must be an core object");
					return JS_FALSE;
				}

				Color *color = dynamic_cast<Color *>(obj);
				if(!color)
				{
					JS_ReportError(ecx()->_jsCx, "DrawableFillColor.color: arg must be a Color object");
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
	const ::Magick::DrawableFillColor &DrawableFillColor::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFillColor::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableFillColor::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableFillColorConstructor::DrawableFillColorConstructor()
		: JsObject(true, "DrawableFillColorConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableFillColorConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableFillColor *from = jsobjFromJsv<DrawableFillColor>(argv[0]);
			if(from)
			{
				*rval = (new DrawableFillColor(*from))->thisJsval();
			}
			else
			{
				DrawableFillColor *res = new DrawableFillColor();
				if(!res->xetter_color(argv, false)) return false;
				*rval = res->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "FillColorConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
