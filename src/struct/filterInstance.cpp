#include "stdafx.h"
#include "struct/filterInstance.hpp"
#include "struct/pointInstance.hpp"
#include "struct/filter.hpp"
#include "router/contextData.hpp"
#include "router/router.hpp"


#include "struct/withMaps.inc.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	EFilterInstanceKind FilterInstance::getKind()
	{
		return _kind;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string FilterInstance::propertyName4Error(PropertyInstance *prop)
	{
		std::string res = "filter property at /" + _parent->getPath().string();

		TMPropertyInstances::iterator iter = _properties.begin();
		TMPropertyInstances::iterator end = _properties.end();

		for(; iter!=end; iter++)
		{
			if(prop == iter->second.get())
			{
				res += ":"+iter->first;
				break;
			}
		}

		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	const TMPropertyInstances &FilterInstance::getProperties()
	{
		return _properties;
	}

	//////////////////////////////////////////////////////////////////////////
	const TSNames &FilterInstance::getProperties_hidden()
	{
		return _properties_hidden;
	}

	//////////////////////////////////////////////////////////////////////////
	void FilterInstance::getProperties_inh(TMPropertyInstances &res)
	{
		//_parent->_super->getFilter(thisName)->getProperties_inh();
		res.seq = _properties.seq;
	}

	//////////////////////////////////////////////////////////////////////////
	FilterInstance::FilterInstance()
		: WithMaps<FilterInstance>(true, "FilterInstance")
		, _parent(NULL)
		, _kind(efikUnknown)
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	FilterInstance::~FilterInstance()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void FilterInstance::setParent(PointInstance *parent)
	{
		_parent = parent;
		BOOST_FOREACH(const TMPropertyInstances::Element &el, _properties.seq)
		{
			el.second->setParent(this);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void FilterInstance::init()
	{
		if(isInited())
		{
			return;
		}

// 		{
// 			ContextDataPtr cd = mkp<ContextData>(new ContextData(this), ROOTNAME);
// 			jsRegisterProp("cd", cd->thisJsval());
// 		}

		JSObject *obj_properties = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		mapInit(_properties.seq, obj_properties);
		jsRegisterProp("properties", OBJECT_TO_JSVAL(obj_properties));

		//////////////////////////////////////////////////////////////////////////
		if(_parent)
		{
			jsRegisterProp("parent", _parent->thisJsval());
		}

		Initializable::init();
	}

	//////////////////////////////////////////////////////////////////////////
	void FilterInstance::deinit()
	{
		if(!isInited())
		{
			return;
		}

		deletePermanent("properties");
		mapDeinit(_properties);

		deletePermanent("parent");


		//getPropertyPtr<ContextData>("cd")->unregister();
		//deletePermanent("cd");

		Initializable::deinit();
	}
}
