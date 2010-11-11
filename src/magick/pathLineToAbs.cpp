#include "magick_pch.h"
#include "magick/pathLineToAbs.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathLineToAbs::PathLineToAbs(const ::Magick::CoordinateList &from)
		: PathBase("[object PathLineToAbs]")
		, _impl(new ::Magick::PathLinetoAbs(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToAbs::PathLineToAbs(const PathLineToAbs &from)
		: PathBase("[object PathLineToAbs]")
		, _impl(new ::Magick::PathLinetoAbs(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToAbs::~PathLineToAbs()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToAbs::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathLineToAbs(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathLinetoAbs &PathLineToAbs::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToAbs::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathLineToAbs::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathLineToAbsConstructor::PathLineToAbsConstructor()
		: JsObject(true, "[object PathLineToAbsConstructor]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathLineToAbsConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathLineToAbs *from = jsobjFromJsv<PathLineToAbs>(argv[0]);
			if(from)
			{
				*rval = (new PathLineToAbs(*from))->thisJsval();
			}
			else
			{
				::Magick::CoordinateList list;
				if(!CoordinateList_jsv(argv[0], list)) return false;
				*rval = (new PathLineToAbs(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathLineToAbsConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
