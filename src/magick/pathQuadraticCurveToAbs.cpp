#include "magick_pch.h"
#include "magick/pathQuadraticCurveToAbs.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathQuadraticCurveToAbs::PathQuadraticCurveToAbs(const ::Magick::PathQuadraticCurvetoArgsList &from)
		: PathBase("[object PathQuadraticCurveToAbs]")
		, _impl(new ::Magick::PathQuadraticCurvetoAbs(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathQuadraticCurveToAbs::PathQuadraticCurveToAbs(const PathQuadraticCurveToAbs &from)
		: PathBase("[object PathQuadraticCurveToAbs]")
		, _impl(new ::Magick::PathQuadraticCurvetoAbs(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathQuadraticCurveToAbs::~PathQuadraticCurveToAbs()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathQuadraticCurveToAbs::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathQuadraticCurveToAbs(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathQuadraticCurvetoAbs &PathQuadraticCurveToAbs::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathQuadraticCurveToAbs::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathQuadraticCurveToAbs::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathQuadraticCurveToAbsConstructor::PathQuadraticCurveToAbsConstructor()
		: JsObject(true, "[object PathQuadraticCurveToAbsConstructor]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathQuadraticCurveToAbsConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathQuadraticCurveToAbs *from = jsobjFromJsv<PathQuadraticCurveToAbs>(argv[0]);
			if(from)
			{
				*rval = (new PathQuadraticCurveToAbs(*from))->thisJsval();
			}
			else
			{
				::Magick::PathQuadraticCurvetoArgsList list;
				if(!PathQuadraticCurveToArgsList_jsv(argv[0], list)) return false;
				*rval = (new PathQuadraticCurveToAbs(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathQuadraticCurveToAbsConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
