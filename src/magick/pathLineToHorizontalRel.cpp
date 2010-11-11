#include "magick_pch.h"
#include "magick/pathLineToHorizontalRel.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathLineToHorizontalRel::PathLineToHorizontalRel(double from)
		: PathBase("[object PathLineToHorizontalRel]")
		, _impl(new ::Magick::PathLinetoHorizontalRel(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToHorizontalRel::PathLineToHorizontalRel(const PathLineToHorizontalRel &from)
		: PathBase("[object PathLineToHorizontalRel]")
		, _impl(new ::Magick::PathLinetoHorizontalRel(from.getImpl().x()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToHorizontalRel::~PathLineToHorizontalRel()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToHorizontalRel::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathLineToHorizontalRel(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathLinetoHorizontalRel &PathLineToHorizontalRel::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToHorizontalRel::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathLineToHorizontalRel::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathLineToHorizontalRelConstructor::PathLineToHorizontalRelConstructor()
		: JsObject(true, "[object PathLineToHorizontalRelConstructor]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathLineToHorizontalRelConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathLineToHorizontalRel *from = jsobjFromJsv<PathLineToHorizontalRel>(argv[0]);
			if(from)
			{
				*rval = (new PathLineToHorizontalRel(*from))->thisJsval();
			}
			else
			{
				jsdouble d;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &d)) return false;
				*rval = (new PathLineToHorizontalRel(d))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathLineToHorizontalRelConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
