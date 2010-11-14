#include "magick_pch.h"
#include "magick/pathLineToHorizontalAbs.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathLineToHorizontalAbs::PathLineToHorizontalAbs(double from)
		: PathBase("PathLineToHorizontalAbs")
		, _impl(new ::Magick::PathLinetoHorizontalAbs(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToHorizontalAbs::PathLineToHorizontalAbs(const PathLineToHorizontalAbs &from)
		: PathBase("PathLineToHorizontalAbs")
		, _impl(new ::Magick::PathLinetoHorizontalAbs(from.getImpl().x()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToHorizontalAbs::~PathLineToHorizontalAbs()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToHorizontalAbs::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathLineToHorizontalAbs(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathLinetoHorizontalAbs &PathLineToHorizontalAbs::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToHorizontalAbs::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathLineToHorizontalAbs::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathLineToHorizontalAbsConstructor::PathLineToHorizontalAbsConstructor()
		: JsObject(true, "PathLineToHorizontalAbsConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathLineToHorizontalAbsConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathLineToHorizontalAbs *from = jsobjFromJsv<PathLineToHorizontalAbs>(argv[0]);
			if(from)
			{
				*rval = (new PathLineToHorizontalAbs(*from))->thisJsval();
			}
			else
			{
				jsdouble d;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &d)) return false;
				*rval = (new PathLineToHorizontalAbs(d))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathLineToHorizontalAbsConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
