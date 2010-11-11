#include "stdafx.h"
#include "struct/static.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Static::Static(JsObject *parent, StaticInstancePtr instance)
		: _parent(parent)
		, _instance(instance)
		, JsObject(true, "[object Static]")
	{

	}

	//////////////////////////////////////////////////////////////////////////
	Static::~Static()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void Static::init()
	{
		if(isInited())
		{
			return;
		}


		JSContext *cx = ecx()->_jsCx;

		//////////////////////////////////////////////////////////////////////////
		if(_parent)
		{
			jsRegisterProp("parent", _parent->thisJsval());
		}

		if(_instance)
		{
			jsRegisterProp("instance", _instance->thisJsval());
		}

		(JSExceptionReporter)JS_SetPrototype(cx, thisJsobj(), _instance->thisJsobj());
		//setPrototype(_instance);

		Initializable::init();
	}

	//////////////////////////////////////////////////////////////////////////
	void Static::deinit()
	{
		if(!isInited())
		{
			return;
		}

		Initializable::deinit();

		//setPrototype(JsPtr());
		(JSExceptionReporter)JS_SetPrototype(ecx()->_jsCx, thisJsobj(), NULL);


		deletePermanent("parent");
		deletePermanent("instance");
	}

	//////////////////////////////////////////////////////////////////////////
	const StaticInstancePtr &Static::getInstance()
	{
		return _instance;
	}

	//////////////////////////////////////////////////////////////////////////
	const std::string &Static::getFileName()
	{
		return _instance->getFileName();
	}

}
