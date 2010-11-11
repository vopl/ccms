#include "stdafx.h"
#include "mime/headerDate.hpp"

namespace ccms{namespace mime{
	//////////////////////////////////////////////////////////////////////////
	HeaderDate::HeaderDate()
		: HeaderBase("[object HeaderDate]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	HeaderDate::~HeaderDate()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool HeaderDate::construct(uintN argc, jsval *argv)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool HeaderDate::jsRegister()
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	HeaderDateConstructor::HeaderDateConstructor()
		: JsObject(true, "[object HeaderDateConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool HeaderDateConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		HeaderDate *robj = new HeaderDate;
		if(!robj->construct(argc, argv)) return false;
		*rval = robj->thisJsval();
		return true;
	}
}}
