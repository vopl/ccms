#include "magick_pch.h"
#include "magick/pathLineToVerticalAbs.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathLineToVerticalAbs::PathLineToVerticalAbs(double from)
		: PathBase("PathLineToVerticalAbs")
		, _impl(new ::Magick::PathLinetoVerticalAbs(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToVerticalAbs::PathLineToVerticalAbs(const PathLineToVerticalAbs &from)
		: PathBase("PathLineToVerticalAbs")
		, _impl(new ::Magick::PathLinetoVerticalAbs(from.getImpl().y()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToVerticalAbs::~PathLineToVerticalAbs()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToVerticalAbs::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathLineToVerticalAbs(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathLinetoVerticalAbs &PathLineToVerticalAbs::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToVerticalAbs::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathLineToVerticalAbs::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathLineToVerticalAbsConstructor::PathLineToVerticalAbsConstructor()
		: JsObject(true, "PathLineToVerticalAbsConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathLineToVerticalAbsConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathLineToVerticalAbs *from = jsobjFromJsv<PathLineToVerticalAbs>(argv[0]);
			if(from)
			{
				*rval = (new PathLineToVerticalAbs(*from))->thisJsval();
			}
			else
			{
				jsdouble d;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &d)) return false;
				*rval = (new PathLineToVerticalAbs(d))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathLineToVerticalAbsConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
