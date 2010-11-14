#include "magick_pch.h"
#include "magick/pathCurveToRel.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathCurveToRel::PathCurveToRel(const ::Magick::PathCurveToArgsList &from)
		: PathBase("PathCurveToRel")
		, _impl(new ::Magick::PathCurvetoRel(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathCurveToRel::PathCurveToRel(const PathCurveToRel &from)
		: PathBase("PathCurveToRel")
		, _impl(new ::Magick::PathCurvetoRel(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathCurveToRel::~PathCurveToRel()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathCurveToRel::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathCurveToRel(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathCurvetoRel &PathCurveToRel::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathCurveToRel::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathCurveToRel::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathCurveToRelConstructor::PathCurveToRelConstructor()
		: JsObject(true, "PathCurveToRelConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathCurveToRelConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathCurveToRel *from = jsobjFromJsv<PathCurveToRel>(argv[0]);
			if(from)
			{
				*rval = (new PathCurveToRel(*from))->thisJsval();
			}
			else
			{
				::Magick::PathCurveToArgsList list;
				if(!PathCurveToArgsList_jsv(argv[0], list)) return false;
				*rval = (new PathCurveToRel(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathCurveToRelConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
