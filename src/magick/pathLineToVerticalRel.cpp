#include "magick_pch.h"
#include "magick/pathLineToVerticalRel.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathLineToVerticalRel::PathLineToVerticalRel(double from)
		: PathBase("PathLineToVerticalRel")
		, _impl(new ::Magick::PathLinetoVerticalRel(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToVerticalRel::PathLineToVerticalRel(const PathLineToVerticalRel &from)
		: PathBase("PathLineToVerticalRel")
		, _impl(new ::Magick::PathLinetoVerticalRel(from.getImpl().y()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathLineToVerticalRel::~PathLineToVerticalRel()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToVerticalRel::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathLineToVerticalRel(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathLinetoVerticalRel &PathLineToVerticalRel::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathLineToVerticalRel::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathLineToVerticalRel::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathLineToVerticalRelConstructor::PathLineToVerticalRelConstructor()
		: JsObject(true, "PathLineToVerticalRelConstructor")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathLineToVerticalRelConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathLineToVerticalRel *from = jsobjFromJsv<PathLineToVerticalRel>(argv[0]);
			if(from)
			{
				*rval = (new PathLineToVerticalRel(*from))->thisJsval();
			}
			else
			{
				jsdouble d;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &d)) return false;
				*rval = (new PathLineToVerticalRel(d))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathLineToVerticalRelConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
