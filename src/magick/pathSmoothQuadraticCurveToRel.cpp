#include "magick_pch.h"
#include "magick/pathSmoothQuadraticCurveToRel.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathSmoothQuadraticCurveToRel::PathSmoothQuadraticCurveToRel(const ::Magick::CoordinateList &from)
		: PathBase("PathSmoothQuadraticCurveToRel")
		, _impl(new ::Magick::PathSmoothQuadraticCurvetoRel(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathSmoothQuadraticCurveToRel::PathSmoothQuadraticCurveToRel(const PathSmoothQuadraticCurveToRel &from)
		: PathBase("PathSmoothQuadraticCurveToRel")
		, _impl(new ::Magick::PathSmoothQuadraticCurvetoRel(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathSmoothQuadraticCurveToRel::~PathSmoothQuadraticCurveToRel()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathSmoothQuadraticCurveToRel::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathSmoothQuadraticCurveToRel(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathSmoothQuadraticCurvetoRel &PathSmoothQuadraticCurveToRel::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathSmoothQuadraticCurveToRel::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathSmoothQuadraticCurveToRel::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathSmoothQuadraticCurveToRelConstructor::PathSmoothQuadraticCurveToRelConstructor()
		: JsObject(true, "PathSmoothQuadraticCurveToRelConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathSmoothQuadraticCurveToRelConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathSmoothQuadraticCurveToRel *from = jsobjFromJsv<PathSmoothQuadraticCurveToRel>(argv[0]);
			if(from)
			{
				*rval = (new PathSmoothQuadraticCurveToRel(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				*rval = (new PathSmoothQuadraticCurveToRel(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathSmoothQuadraticCurveToRelConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
