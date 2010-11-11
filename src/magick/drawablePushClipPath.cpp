#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawablePushClipPath.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePushClipPath::DrawablePushClipPath(const char *id_)
		: DrawableBase("[object DrawablePushClipPath]")
		, _impl(new ::Magick::DrawablePushClipPath(id_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePushClipPath::DrawablePushClipPath(const DrawablePushClipPath &from)
		: DrawableBase("[object DrawablePushClipPath]")
		, _impl(new ::Magick::DrawablePushClipPath(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePushClipPath::~DrawablePushClipPath()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePushClipPath::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePushClipPath(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
// 	//////////////////////////////////////////////////////////////////////////
// 	bool DrawableFont::xetter_id(jsval *vp, bool isGet)
// 	{
// 		if(isGet)
// 		{
// 			std::string str = _impl->id();
// 			*vp = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, str.data(), str.size()));
// 		}
// 		else
// 		{
// 			char *str;
// 			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &str)) return false;
// 			IMTRY()
// 				_impl->id(str);
// 			IMCATCH(return false);
// 		}
// 		return true;
// 	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePushClipPath &DrawablePushClipPath::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePushClipPath::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePushClipPath::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePushClipPathConstructor::DrawablePushClipPathConstructor()
		: JsObject(true, "[object DrawablePushClipPathConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePushClipPathConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawablePushClipPath *from = jsobjFromJsv<DrawablePushClipPath>(argv[0]);
			if(from)
			{
				*rval = (new DrawablePushClipPath(*from))->thisJsval();
			}
			else
			{
				char *str;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &str)) return false;
				*rval = (new DrawablePushClipPath(str))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PushClipPathConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
