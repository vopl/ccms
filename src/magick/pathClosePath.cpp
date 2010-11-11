#include "magick_pch.h"
#include "magick/pathClosePath.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathClosePath::PathClosePath()
		: PathBase("[object PathClosePath]")
		, _impl(new ::Magick::PathClosePath)
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathClosePath::~PathClosePath()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathClosePath::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathClosePath)->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathClosePath &PathClosePath::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathClosePath::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathClosePath::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathClosePathConstructor::PathClosePathConstructor()
		: JsObject(true, "[object PathClosePathConstructor]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathClosePathConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(0 == argc)
		{
			*rval = (new PathClosePath)->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathClosePathConstructor: must be called with 0 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
