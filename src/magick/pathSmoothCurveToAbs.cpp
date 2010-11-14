#include "magick_pch.h"
#include "magick/pathSmoothCurveToAbs.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathSmoothCurveToAbs::PathSmoothCurveToAbs(const ::Magick::CoordinateList &from)
		: PathBase("PathSmoothCurveToAbs")
		, _impl(new ::Magick::PathSmoothCurvetoAbs(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathSmoothCurveToAbs::PathSmoothCurveToAbs(const PathSmoothCurveToAbs &from)
		: PathBase("PathSmoothCurveToAbs")
		, _impl(new ::Magick::PathSmoothCurvetoAbs(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathSmoothCurveToAbs::~PathSmoothCurveToAbs()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathSmoothCurveToAbs::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathSmoothCurveToAbs(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathSmoothCurvetoAbs &PathSmoothCurveToAbs::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathSmoothCurveToAbs::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathSmoothCurveToAbs::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathSmoothCurveToAbsConstructor::PathSmoothCurveToAbsConstructor()
		: JsObject(true, "PathSmoothCurveToAbsConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathSmoothCurveToAbsConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathSmoothCurveToAbs *from = jsobjFromJsv<PathSmoothCurveToAbs>(argv[0]);
			if(from)
			{
				*rval = (new PathSmoothCurveToAbs(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;

				if(list.size() % 2)
				{
					JS_ReportError(ecx()->_jsCx, "PathSmoothCurveToAbsConstructor amount of coordinates must be even");
					return false;
				}
				*rval = (new PathSmoothCurveToAbs(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathSmoothCurveToAbsConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
