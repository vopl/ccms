#include "magick_pch.h"
#include "magick/pathSmoothCurveToRel.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathSmoothCurveToRel::PathSmoothCurveToRel(const ::Magick::CoordinateList &from)
		: PathBase("PathSmoothCurveToRel")
		, _impl(new ::Magick::PathSmoothCurvetoRel(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathSmoothCurveToRel::PathSmoothCurveToRel(const PathSmoothCurveToRel &from)
		: PathBase("PathSmoothCurveToRel")
		, _impl(new ::Magick::PathSmoothCurvetoRel(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathSmoothCurveToRel::~PathSmoothCurveToRel()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathSmoothCurveToRel::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathSmoothCurveToRel(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathSmoothCurvetoRel &PathSmoothCurveToRel::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathSmoothCurveToRel::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathSmoothCurveToRel::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathSmoothCurveToRelConstructor::PathSmoothCurveToRelConstructor()
		: JsObject(true, "PathSmoothCurveToRelConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathSmoothCurveToRelConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathSmoothCurveToRel *from = jsobjFromJsv<PathSmoothCurveToRel>(argv[0]);
			if(from)
			{
				*rval = (new PathSmoothCurveToRel(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				if(list.size() % 2)
				{
					JS_ReportError(ecx()->_jsCx, "PathSmoothCurveToRelConstructor amount of coordinates must be even");
					return false;
				}

				*rval = (new PathSmoothCurveToRel(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathSmoothCurveToRelConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
