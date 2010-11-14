#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableClipPath.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableClipPath::DrawableClipPath(const char *from)
		: DrawableBase("DrawableClipPath")
		, _impl(new ::Magick::DrawableClipPath(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableClipPath::DrawableClipPath(const DrawableClipPath &from)
		: DrawableBase("DrawableClipPath")
		, _impl(new ::Magick::DrawableClipPath(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableClipPath::~DrawableClipPath()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableClipPath::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableClipPath(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableClipPath::xetter_path(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			std::string str = _impl->clip_path();
			*vp = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, str.data(), str.size()));
		}
		else
		{
			char *str;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &str)) return false;
			_impl->clip_path(str);
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableClipPath &DrawableClipPath::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableClipPath::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableClipPath::call_copy, 0))return false;

		if(!jsRegisterProp("path", &DrawableClipPath::xetter_path, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableClipPathConstructor::DrawableClipPathConstructor()
		: JsObject(true, "DrawableClipPathConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableClipPathConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableClipPath *from = jsobjFromJsv<DrawableClipPath>(argv[0]);
			if(from)
			{
				*rval = (new DrawableClipPath(*from))->thisJsval();
			}
			else
			{
				char *str;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &str)) return false;
				*rval = (new DrawableClipPath(str))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "ClipPathConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
