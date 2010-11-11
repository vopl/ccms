#include "stdafx.h"
#include "mime/headerBase.hpp"

namespace ccms{namespace mime{
	
	//////////////////////////////////////////////////////////////////////////
	HeaderBase::HeaderBase(const char*jsName)
		: JsObject(true, jsName)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	HeaderBase::~HeaderBase()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	const char *HeaderBase::getName() const
	{
		if(_name.empty()) return "";
		return _name.data();
	}

	//////////////////////////////////////////////////////////////////////////
	void HeaderBase::setName(const char *name)
	{
		_name = name;
	}



}}

