#include "magick_pch.h"
#include "magick/drawablePolygon.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawablePolygon::DrawablePolygon(const ::Magick::CoordinateList &list)
		: DrawableBase("[object DrawablePolygon]")
		, _impl(new ::Magick::DrawablePolygon(list))
	{
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePolygon::DrawablePolygon(const DrawablePolygon &from)
		: DrawableBase("[object DrawablePolygon]")
		, _impl(new ::Magick::DrawablePolygon(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawablePolygon::~DrawablePolygon()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePolygon::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawablePolygon(*this))->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawablePolygon &DrawablePolygon::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawablePolygon::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawablePolygon::call_copy, 0))return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawablePolygonConstructor::DrawablePolygonConstructor()
		: JsObject(true, "[object DrawablePolygonConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawablePolygonConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawablePolygon *from = jsobjFromJsv<DrawablePolygon>(argv[0]);
			if(from)
			{
				*rval = (new DrawablePolygon(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				*rval = (new DrawablePolygon(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PolygonConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;

	}


}}
