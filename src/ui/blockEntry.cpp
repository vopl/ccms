#include "stdafx.h"
#include "ui/blockEntry.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	BlockEntry::BlockEntry()
		: JsObject(true, "BlockEntry")
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, es_priority, INT_TO_JSVAL(0));

		(JSExceptionReporter)jsRegisterProp("content", &BlockEntry::xetter_content, false);
		(JSExceptionReporter)jsRegisterProp("priority", &BlockEntry::xetter_priority, false);
	}

	//////////////////////////////////////////////////////////////////////////
	BlockEntry::~BlockEntry()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool BlockEntry::xetter_content(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, es_content, vp)?true:false;
		}
		return JS_SetReservedSlot(ecx()->_jsCx, _js, es_content, *vp)?true:false;
	}

	bool BlockEntry::xetter_priority(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, es_priority, vp)?true:false;
		}
		return JS_SetReservedSlot(ecx()->_jsCx, _js, es_priority, *vp)?true:false;
	}


	//////////////////////////////////////////////////////////////////////////
	JSBool BlockEntry::onPropAdd(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool BlockEntry::onPropDel(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool BlockEntry::onPropSet(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool BlockEntry::onPropGet(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}
}
