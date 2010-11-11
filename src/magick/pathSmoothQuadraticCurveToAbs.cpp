#include "magick_pch.h"
#include "magick/pathSmoothQuadraticCurveToAbs.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathSmoothQuadraticCurveToAbs::PathSmoothQuadraticCurveToAbs(const ::Magick::CoordinateList &from)
		: PathBase("[object PathSmoothQuadraticCurveToAbs]")
		, _impl(new ::Magick::PathSmoothQuadraticCurvetoAbs(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathSmoothQuadraticCurveToAbs::PathSmoothQuadraticCurveToAbs(const PathSmoothQuadraticCurveToAbs &from)
		: PathBase("[object PathSmoothQuadraticCurveToAbs]")
		, _impl(new ::Magick::PathSmoothQuadraticCurvetoAbs(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathSmoothQuadraticCurveToAbs::~PathSmoothQuadraticCurveToAbs()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathSmoothQuadraticCurveToAbs::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathSmoothQuadraticCurveToAbs(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathSmoothQuadraticCurvetoAbs &PathSmoothQuadraticCurveToAbs::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathSmoothQuadraticCurveToAbs::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathSmoothQuadraticCurveToAbs::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathSmoothQuadraticCurveToAbsConstructor::PathSmoothQuadraticCurveToAbsConstructor()
		: JsObject(true, "[object PathSmoothQuadraticCurveToAbsConstructor]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathSmoothQuadraticCurveToAbsConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathSmoothQuadraticCurveToAbs *from = jsobjFromJsv<PathSmoothQuadraticCurveToAbs>(argv[0]);
			if(from)
			{
				*rval = (new PathSmoothQuadraticCurveToAbs(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				*rval = (new PathSmoothQuadraticCurveToAbs(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathSmoothQuadraticCurveToAbsConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
