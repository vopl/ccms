#include "magick_pch.h"
#include "magick/pathArcRel.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/pathCnvtHelpers.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	PathArcRel::PathArcRel(const ::Magick::PathArcArgsList &from)
		: PathBase("[object PathArcRel]")
		, _impl(new ::Magick::PathArcRel(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathArcRel::PathArcRel(const PathArcRel &from)
		: PathBase("[object PathArcRel]")
		, _impl(new ::Magick::PathArcRel(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	PathArcRel::~PathArcRel()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool PathArcRel::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new PathArcRel(*this))->thisJsval();
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	const ::Magick::PathArcRel &PathArcRel::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PathArcRel::registerJs()
	{
		if(!jsRegisterMeth("copy", &PathArcRel::call_copy, 0))return false;

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	PathArcRelConstructor::PathArcRelConstructor()
		: JsObject(true, "[object PathArcRelConstructor]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	JSBool PathArcRelConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			PathArcRel *from = jsobjFromJsv<PathArcRel>(argv[0]);
			if(from)
			{
				*rval = (new PathArcRel(*from))->thisJsval();
			}
			else
			{
				::Magick::PathArcArgsList list;
				if(!PathArcArgsList_jsv(argv[0], list)) return false;
				*rval = (new PathArcRel(list))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "PathArcRelConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}

}}
