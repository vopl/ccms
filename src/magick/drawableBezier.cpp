#include "magick_pch.h"
#include "magick/drawableBezier.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableBezier::DrawableBezier(const ::Magick::CoordinateList &list)
		: DrawableBase("[object DrawableBezier]")
		, _impl(new ::Magick::DrawableBezier(list))
	{
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableBezier::DrawableBezier(const DrawableBezier &from)
		: DrawableBase("[object DrawableBezier]")
		, _impl(new ::Magick::DrawableBezier(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableBezier::~DrawableBezier()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableBezier::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableBezier(*this))->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableBezier &DrawableBezier::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableBezier::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableBezier::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableBezierConstructor::DrawableBezierConstructor()
		: JsObject(true, "[object DrawableBezierConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableBezierConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableBezier *from = jsobjFromJsv<DrawableBezier>(argv[0]);
			if(from)
			{
				*rval = (new DrawableBezier(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				*rval = (new DrawableBezier(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "BezierConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;

	}


}}
