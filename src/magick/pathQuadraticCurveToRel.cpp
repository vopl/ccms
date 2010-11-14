#include "magick_pch.h"
#include "magick/pathQuadraticCurveToRel.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathQuadraticCurveToRel::PathQuadraticCurveToRel(const ::Magick::PathQuadraticCurvetoArgsList &from)
		: PathBase("PathQuadraticCurveToRel")
		, _impl(new ::Magick::PathQuadraticCurvetoRel(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathQuadraticCurveToRel::PathQuadraticCurveToRel(const PathQuadraticCurveToRel &from)
		: PathBase("PathQuadraticCurveToRel")
		, _impl(new ::Magick::PathQuadraticCurvetoRel(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathQuadraticCurveToRel::~PathQuadraticCurveToRel()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathQuadraticCurveToRel::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathQuadraticCurveToRel(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathQuadraticCurvetoRel &PathQuadraticCurveToRel::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathQuadraticCurveToRel::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathQuadraticCurveToRel::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathQuadraticCurveToRelConstructor::PathQuadraticCurveToRelConstructor()
		: JsObject(true, "PathQuadraticCurveToRelConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathQuadraticCurveToRelConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathQuadraticCurveToRel *from = jsobjFromJsv<PathQuadraticCurveToRel>(argv[0]);
			if(from)
			{
				*rval = (new PathQuadraticCurveToRel(*from))->thisJsval();
			}
			else
			{
				::Magick::PathQuadraticCurvetoArgsList list;
				if(!PathQuadraticCurveToArgsList_jsv(argv[0], list)) return false;
				*rval = (new PathQuadraticCurveToRel(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathQuadraticCurveToRelConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
