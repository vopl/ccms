#include "magick_pch.h"
#include "magick/pathCurveToAbs.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathCurveToAbs::PathCurveToAbs(const ::Magick::PathCurveToArgsList &from)
		: PathBase("PathCurveToAbs")
		, _impl(new ::Magick::PathCurvetoAbs(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathCurveToAbs::PathCurveToAbs(const PathCurveToAbs &from)
		: PathBase("PathCurveToAbs")
		, _impl(new ::Magick::PathCurvetoAbs(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathCurveToAbs::~PathCurveToAbs()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathCurveToAbs::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathCurveToAbs(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathCurvetoAbs &PathCurveToAbs::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathCurveToAbs::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathCurveToAbs::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathCurveToAbsConstructor::PathCurveToAbsConstructor()
		: JsObject(true, "PathCurveToAbsConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathCurveToAbsConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathCurveToAbs *from = jsobjFromJsv<PathCurveToAbs>(argv[0]);
			if(from)
			{
				*rval = (new PathCurveToAbs(*from))->thisJsval();
			}
			else
			{
				::Magick::PathCurveToArgsList list;
				if(!PathCurveToArgsList_jsv(argv[0], list)) return false;
				*rval = (new PathCurveToAbs(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathCurveToAbsConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
