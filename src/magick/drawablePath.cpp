#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawablePath.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePath::DrawablePath(const ::Magick::VPathList &from)
		: DrawableBase("[object DrawablePath]")
		, _impl(new ::Magick::DrawablePath(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePath::DrawablePath(const DrawablePath &from)
		: DrawableBase("[object DrawablePath]")
		, _impl(new ::Magick::DrawablePath(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePath::~DrawablePath()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePath::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePath(*this))->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePath &DrawablePath::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePath::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePath::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePathConstructor::DrawablePathConstructor()
		: JsObject(true, "[object DrawablePathConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePathConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawablePath *from = jsobjFromJsv<DrawablePath>(argv[0]);
			if(from)
			{
				*rval = (new DrawablePath(*from))->thisJsval();
			}
			else
			{
				::Magick::VPathList list;
				if(!PathList_jsv(argv[0], list)) return false;
				*rval = (new DrawablePath(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
