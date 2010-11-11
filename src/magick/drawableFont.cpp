#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableFont.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableFont::DrawableFont(const char *font_)
		: DrawableBase("[object DrawableFont]")
		, _impl(new ::Magick::DrawableFont(font_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFont::DrawableFont(const char *family_, ::Magick::StyleType style_, uint32 weight_, ::Magick::StretchType stretch_)
		: DrawableBase("[object DrawableFont]")
		, _impl(new ::Magick::DrawableFont(family_, style_, weight_, stretch_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFont::DrawableFont(const DrawableFont &from)
		: DrawableBase("[object DrawableFont]")
		, _impl(new ::Magick::DrawableFont(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableFont::~DrawableFont()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFont::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableFont(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFont::xetter_font(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			std::string str = _impl->font();
			*vp = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, str.data(), str.size()));
		}
		else
		{
			char *str;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &str)) return false;
			IMTRY()
				_impl->font(str);
			IMCATCH(return false);
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableFont &DrawableFont::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableFont::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableFont::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableFontConstructor::DrawableFontConstructor()
		: JsObject(true, "[object DrawableFontConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableFontConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableFont *from = jsobjFromJsv<DrawableFont>(argv[0]);
			if(from)
			{
				*rval = (new DrawableFont(*from))->thisJsval();
			}
			else
			{
				char *str;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &str)) return false;

				*rval = (new DrawableFont(str))->thisJsval();
			}
		}
		else if(4 == argc)
		{
			const char *family_;
			uint32 style_;
			uint32 weight_;
			uint32 stretch_;

			if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "suuu", &family_, &style_, &weight_, &stretch_)) return false;
			*rval = (new DrawableFont(family_, (::Magick::StyleType)style_, weight_, (::Magick::StretchType)stretch_))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "FontConstructor: must be called with 1 or 4 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
