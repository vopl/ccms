#include "stdafx.h"
#include "router/contextDataStorage.hpp"
#include "router/contextData.hpp"
#include "router/execContext.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	ContextDataStorage::ContextDataStorage()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	ContextDataStorage::~ContextDataStorage()
	{
		reset();
	}

	//////////////////////////////////////////////////////////////////////////
	bool ContextDataStorage::startInstance(const std::string &key)
	{
		flushUnregisterCarriers();
		if(_instances.end() != _instances.find(key))
		{
			return false;
		}

		_instances[key] = OBJECT_TO_JSVAL(JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL));
		JS_AddNamedRoot(ecx()->_jsCx, &_instances[key], __FUNCTION__);
		flushUnregisterCarriers();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ContextDataStorage::stopInstance(const std::string &key)
	{
		flushUnregisterCarriers();
		TMInstances::iterator iter = _instances.find(key);
		if(_instances.end() == iter)
		{
			return false;
		}

		BOOST_FOREACH(ContextData *cd, _cds)
		{
			cd->instanceStopped(key.c_str());
		}

		JS_RemoveRoot(ecx()->_jsCx, &iter->second);
		flushUnregisterCarriers();
		_instances.erase(iter);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void ContextDataStorage::reset()
	{
		flushUnregisterCarriers();
		BOOST_FOREACH(TMInstances::value_type &v, _instances)
		{
			JS_RemoveRoot(ecx()->_jsCx, &v.second);
		}
		flushUnregisterCarriers();
		_instances.clear();

		while(_cds.size())
		{
			unregisterCd(*_cds.begin());
		}
	}


	//////////////////////////////////////////////////////////////////////////
	jsval ContextDataStorage::getInstance(const std::string &key)
	{
		TMInstances::iterator iter = _instances.find(key);
		if(_instances.end() == iter)
		{
			return JSVAL_VOID;
		}
		return iter->second;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ContextDataStorage::setInstance(const std::string &key, jsval v)
	{
		if(!JSVAL_IS_OBJECT(v) || JSVAL_IS_NULL(v))
		{
			return false;
		}

		TMInstances::iterator iter = _instances.find(key);
		if(_instances.end() == iter)
		{
			return false;
		}
		//JS_RemoveRoot(ecx()->_jsCx, &iter->second);
		iter->second = v;
		//JS_AddNamedRoot(ecx()->_jsCx, &iter->second, __FUNCTION__);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	const ContextDataStorage::TMInstances ContextDataStorage::getInstances()
	{
		return _instances;
	}

	//////////////////////////////////////////////////////////////////////////
	void ContextDataStorage::registerCd(ContextData *cd)
	{
		_cds.insert(cd);
	}
	
	//////////////////////////////////////////////////////////////////////////
	void ContextDataStorage::unregisterCd(ContextData *cd)
	{
		if(_cds.end() == _cds.find(cd))
		{
			return;
		}

		//вычистить информацию данного носителя из всех инстансов
		const std::string &carrier = cd->getCarrier();
		BOOST_FOREACH(const TMInstances::value_type &p, _instances)
		{
			(JSExceptionReporter)JS_DeleteProperty(ecx()->_jsCx, JSVAL_TO_OBJECT(p.second), carrier.c_str());
		}
		flushUnregisterCarriers();
		_cds.erase(cd);
	}

	//////////////////////////////////////////////////////////////////////////
	void ContextDataStorage::unregisterCd_gcPossible(ContextData *cd)
	{
		if(_cds.end() == _cds.find(cd))
		{
			return;
		}
		_unregisteredCarriers.insert(cd->getCarrier());
		_cds.erase(cd);
	}

	//////////////////////////////////////////////////////////////////////////
	void ContextDataStorage::flushUnregisterCarriers()
	{
		if(!_unregisteredCarriers.empty())
		{
			BOOST_FOREACH(const TSCarriers::value_type &c, _unregisteredCarriers)
			{
				BOOST_FOREACH(const TMInstances::value_type &p, _instances)
				{
					(JSExceptionReporter)JS_DeleteProperty(ecx()->_jsCx, JSVAL_TO_OBJECT(p.second), c.c_str());
				}
			}
			_unregisteredCarriers.clear();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	JSObject *ContextDataStorage::getData4Carrier(const std::string &key, jsid carrier)
	{
		//////////////////////////////////////////////////////////////////////////
		TMInstances::iterator iter = _instances.find(key);
		if(_instances.end() == iter)
		{
			return NULL;
		}

		JSObject *instance = JSVAL_TO_OBJECT(iter->second);

		jsval res;
		if(!JS_GetPropertyById(ecx()->_jsCx, instance, carrier, &res))
		{
			res = OBJECT_TO_JSVAL(JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL));
			if(!JS_DefinePropertyById(ecx()->_jsCx, instance, carrier, res, NULL, NULL, JSPROP_ENUMERATE))return false;
		}

		if(!JSVAL_IS_OBJECT(res) || JSVAL_IS_NULL(res))
		{
			if(!JS_DeletePropertyById(ecx()->_jsCx, instance, carrier))return false;
			res = OBJECT_TO_JSVAL(JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL));
			if(!JS_DefinePropertyById(ecx()->_jsCx, instance, carrier, res, NULL, NULL, JSPROP_ENUMERATE))return false;
		}

		assert(JSVAL_IS_OBJECT(res));
		return JSVAL_TO_OBJECT(res);
	}

	//////////////////////////////////////////////////////////////////////////
	void ContextDataStorage::resetData4Carrier(const std::string &key, jsid carrier)
	{
		TMInstances::iterator iter = _instances.find(key);
		if(_instances.end() == iter)
		{
			return;
		}

		JSObject *instance = JSVAL_TO_OBJECT(iter->second);

		(JSExceptionReporter)JS_DeletePropertyById(ecx()->_jsCx, instance, carrier);
	}

}
