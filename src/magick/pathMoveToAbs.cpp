#include "magick_pch.h"
#include "magick/pathMoveToAbs.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathMoveToAbs::PathMoveToAbs(const ::Magick::CoordinateList &from)
		: PathBase("PathMoveToAbs")
		, _impl(new ::Magick::PathMovetoAbs(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathMoveToAbs::PathMoveToAbs(const PathMoveToAbs &from)
		: PathBase("PathMoveToAbs")
		, _impl(new ::Magick::PathMovetoAbs(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathMoveToAbs::~PathMoveToAbs()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathMoveToAbs::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathMoveToAbs(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathMovetoAbs &PathMoveToAbs::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathMoveToAbs::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathMoveToAbs::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathMoveToAbsConstructor::PathMoveToAbsConstructor()
		: JsObject(true, "PathMoveToAbsConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathMoveToAbsConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathMoveToAbs *from = jsobjFromJsv<PathMoveToAbs>(argv[0]);
			if(from)
			{
				*rval = (new PathMoveToAbs(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				*rval = (new PathMoveToAbs(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathMoveToAbsConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
