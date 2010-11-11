#include "magick_pch.h"
#include "magick/pathArcAbs.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathArcAbs::PathArcAbs(const ::Magick::PathArcArgsList &from)
		: PathBase("[object PathArcAbs]")
		, _impl(new ::Magick::PathArcAbs(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathArcAbs::PathArcAbs(const PathArcAbs &from)
		: PathBase("[object PathArcAbs]")
		, _impl(new ::Magick::PathArcAbs(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathArcAbs::~PathArcAbs()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathArcAbs::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathArcAbs(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathArcAbs &PathArcAbs::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathArcAbs::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathArcAbs::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathArcAbsConstructor::PathArcAbsConstructor()
		: JsObject(true, "[object PathArcAbsConstructor]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathArcAbsConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathArcAbs *from = jsobjFromJsv<PathArcAbs>(argv[0]);
			if(from)
			{
				*rval = (new PathArcAbs(*from))->thisJsval();
			}
			else
			{
				::Magick::PathArcArgsList list;
				if(!PathArcArgsList_jsv(argv[0], list)) return false;
				*rval = (new PathArcAbs(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathArcAbsConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
