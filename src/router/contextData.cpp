#include "stdafx.h"
#include "router/contextData.hpp"
#include "router/contextDataStorage.hpp"
#include "router/execContext.hpp"

namespace ccms
{


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//вспомогательный проксь
	class ContextData2
		: public JsObject
	{
	public:
		ContextData2(const char *key, const char *carrier);
		~ContextData2();

	private:
		std::string	_key;
		JSString	*_carrierStr;
		jsid		_carrierId;
		bool		_isInternalDel;
		virtual JSBool onPropAdd(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropDel(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropSet(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropGet(JSObject *obj, jsid id, jsval *vp);

		virtual JSBool onEnumerate();

	};

	typedef JsPtr<ContextData2> ContextData2Ptr;














	//////////////////////////////////////////////////////////////////////////
	ContextData::ContextData(const void *carrier)
		: JsObject(true, "ContextData")
	{
		char tmp[128];
		sprintf(tmp, "%p", carrier);
		_carrier = tmp;
		ecx()->_cds->registerCd(this);

		//onEnumerate();
	}

	//////////////////////////////////////////////////////////////////////////
	ContextData::~ContextData()
	{
		unregister();
	}

	//////////////////////////////////////////////////////////////////////////
	void ContextData::instanceStopped(const char *key)
	{
		deletePermanent(key);
	}

	//////////////////////////////////////////////////////////////////////////
	const std::string &ContextData::getCarrier()
	{
		return _carrier;
	}

	//////////////////////////////////////////////////////////////////////////
	void ContextData::unregister()
	{
		ecx()->_cds->unregisterCd_gcPossible(this);
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData::onPropAdd(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData::onPropDel(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData::onPropSet(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData::onPropGet(JSObject *obj, jsid id, jsval *vp)
	{
		if(JSVAL_IS_VOID(*vp))
		{
			char *key;
			jsval idVal;
			if(!JS_IdToValue(ecx()->_jsCx, id, &idVal))return false;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &idVal, "s", &key))return false;

			if(JSVAL_IS_VOID(ecx()->_cds->getInstance(key)))
			{
				//инстанса для такого ключа нет, пусть останется undefined
				return JS_TRUE;
			}

			ContextData2Ptr helper = mkp(new ContextData2(key, _carrier.c_str()), ROOTNAME);
			jsRegisterProp(key, helper->thisJsval(), true);
			*vp = helper->thisJsval();
		}

		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData::onEnumerate()
	{
		const ContextDataStorage::TMInstances &instances = ecx()->_cds->getInstances();

		BOOST_FOREACH(const ContextDataStorage::TMInstances::value_type &p, instances)
		{
			const char *key = p.first.c_str();
			ContextData2Ptr helper = mkp(new ContextData2(key, _carrier.c_str()), ROOTNAME);
			jsRegisterProp(key, helper->thisJsval(), true);
		}

		return JS_TRUE;
	}


















	//////////////////////////////////////////////////////////////////////////
	ContextData2::ContextData2(const char *key, const char *carrier)
		: JsObject(true, "ContextData2")
		, _key(key)
		, _carrierStr(JS_NewStringCopyZ(ecx()->_jsCx, carrier))
		, _isInternalDel(false)
	{
		(JSExceptionReporter)JS_ValueToId(ecx()->_jsCx, STRING_TO_JSVAL(_carrierStr), &_carrierId);

		//тут неприятный эффект
		/*
			если ранее создавался jsid из строки с таким же контентом как и эта
			то id всегда навостряется на первую строку
			она может быть удалена в произвольное время но ид все равно на нее ссылается
			поэтому надо взять эту первую строку и забанить у сборщика мусора именно ее
		*/
		jsval jsv;
		(JSExceptionReporter)JS_IdToValue(ecx()->_jsCx, _carrierId, &jsv);
		JSString *newStr = JSVAL_TO_STRING(jsv);
		_carrierStr = newStr;

		JS_AddNamedRoot(ecx()->_jsCx, &_carrierStr, __FUNCTION__);

		//onEnumerate();
	}

	//////////////////////////////////////////////////////////////////////////
	ContextData2::~ContextData2()
	{
		JS_RemoveRoot(ecx()->_jsCx, &_carrierStr);
		_carrierStr = NULL;
		//_carrierId = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData2::onPropAdd(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData2::onPropDel(JSObject *obj, jsid id, jsval *vp)
	{
		if(_isInternalDel)
		{
			return JS_TRUE;
		}

		JSObject *data = ecx()->_cds->getData4Carrier(_key, _carrierId);
		if(!data)
		{
			//no error, ignore
			return JS_TRUE;
		}

		_isInternalDel = true;
		if(!JS_DeletePropertyById(ecx()->_jsCx, thisJsobj(), id))return false;
		if(!JS_DeletePropertyById(ecx()->_jsCx, data, id))return false;
		_isInternalDel = false;

		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData2::onPropSet(JSObject *obj, jsid id, jsval *vp)
	{
		JSObject *data = ecx()->_cds->getData4Carrier(_key, _carrierId);
		if(!data)
		{
			//no error, ignore
			return JS_TRUE;
		}
		if(!JS_DefinePropertyById(ecx()->_jsCx, data, id, *vp, NULL, NULL, JSPROP_ENUMERATE))return false;

		_isInternalDel = true;
		if(!JS_DeletePropertyById(ecx()->_jsCx, thisJsobj(), id))return false;
		_isInternalDel = false;
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData2::onPropGet(JSObject *obj, jsid id, jsval *vp)
	{
		if(!JSVAL_IS_VOID(*vp))
		{
			return JS_TRUE;
		}

		JSObject *data = ecx()->_cds->getData4Carrier(_key, _carrierId);
		if(!data)
		{
			//no error, ignore
			return JS_TRUE;
		}

		if(!JS_GetPropertyById(ecx()->_jsCx, data, id, vp))return false;

		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ContextData2::onEnumerate()
	{
		JSObject *data = ecx()->_cds->getData4Carrier(_key, _carrierId);
		if(!data)
		{
			return JS_TRUE;
		}

		JSIdArray *ids = JS_Enumerate(ecx()->_jsCx, data);
		for(jsint i(0); i<ids->length; i++)
		{
			if(!JS_DefinePropertyById(ecx()->_jsCx, thisJsobj(), ids->vector[i], JSVAL_VOID, NULL, NULL, JSPROP_ENUMERATE))return false;
		}

		JS_DestroyIdArray(ecx()->_jsCx, ids);

		return JS_TRUE;
	}

}
