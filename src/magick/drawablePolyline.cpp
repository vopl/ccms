#include "magick_pch.h"
#include "magick/drawablePolyline.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePolyline::DrawablePolyline(const ::Magick::CoordinateList &list)
		: DrawableBase("DrawablePolyline")
		, _impl(new ::Magick::DrawablePolyline(list))
	{
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePolyline::DrawablePolyline(const DrawablePolyline &from)
		: DrawableBase("DrawablePolyline")
		, _impl(new ::Magick::DrawablePolyline(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePolyline::~DrawablePolyline()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePolyline::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePolyline(*this))->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePolyline &DrawablePolyline::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePolyline::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePolyline::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePolylineConstructor::DrawablePolylineConstructor()
		: JsObject(true, "DrawablePolylineConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePolylineConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawablePolyline *from = jsobjFromJsv<DrawablePolyline>(argv[0]);
			if(from)
			{
				*rval = (new DrawablePolyline(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				*rval = (new DrawablePolyline(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PolylineConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;

	}


}}
