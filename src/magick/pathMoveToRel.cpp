#include "magick_pch.h"
#include "magick/pathMoveToRel.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathMoveToRel::PathMoveToRel(const ::Magick::CoordinateList &from)
		: PathBase("[object PathMoveToRel]")
		, _impl(new ::Magick::PathMovetoRel(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathMoveToRel::PathMoveToRel(const PathMoveToRel &from)
		: PathBase("[object PathMoveToRel]")
		, _impl(new ::Magick::PathMovetoRel(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathMoveToRel::~PathMoveToRel()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathMoveToRel::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathMoveToRel(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathMovetoRel &PathMoveToRel::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathMoveToRel::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathMoveToRel::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathMoveToRelConstructor::PathMoveToRelConstructor()
		: JsObject(true, "[object PathMoveToRelConstructor]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathMoveToRelConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathMoveToRel *from = jsobjFromJsv<PathMoveToRel>(argv[0]);
			if(from)
			{
				*rval = (new PathMoveToRel(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				*rval = (new PathMoveToRel(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathMoveToRelConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
