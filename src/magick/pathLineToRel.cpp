#include "magick_pch.h"
#include "magick/pathLineToRel.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathLineToRel::PathLineToRel(const ::Magick::CoordinateList &from)
		: PathBase("PathLineToRel")
		, _impl(new ::Magick::PathLinetoRel(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToRel::PathLineToRel(const PathLineToRel &from)
		: PathBase("PathLineToRel")
		, _impl(new ::Magick::PathLinetoRel(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToRel::~PathLineToRel()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToRel::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathLineToRel(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathLinetoRel &PathLineToRel::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToRel::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathLineToRel::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathLineToRelConstructor::PathLineToRelConstructor()
		: JsObject(true, "PathLineToRelConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathLineToRelConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathLineToRel *from = jsobjFromJsv<PathLineToRel>(argv[0]);
			if(from)
			{
				*rval = (new PathLineToRel(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				*rval = (new PathLineToRel(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathLineToRelConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
