#include "stdafx.h"
#include "struct/property.hpp"
#include "router/request.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Property::Property(JsObject *parent, PropertyInstancePtr instance)
		: _parent(parent)
		, _instance(instance)
		, JsObject(true, "Property")
	{

		jsRegisterMeth("render", &Property::call_render, 0);
	}

	//////////////////////////////////////////////////////////////////////////
	Property::~Property()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void Property::init()
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

		//setPrototype(_instance);
		(JSExceptionReporter)JS_SetPrototype(ecx()->_jsCx, thisJsobj(), _instance->thisJsobj());

		Initializable::init();
	}

	//////////////////////////////////////////////////////////////////////////
	void Property::deinit()
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
	const PropertyInstancePtr &Property::getInstance()
	{
		return _instance;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Property::call_render(uintN argc, jsval *argv, jsval *rval)
	{
		return _instance->render(_parent, argc, argv, rval);
	}


	//////////////////////////////////////////////////////////////////////////
	JSBool Property::onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		return _instance->render(_parent, argc, argv, rval)?JS_TRUE:JS_FALSE;
	}

}
