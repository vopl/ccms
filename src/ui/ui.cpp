#include "stdafx.h"
#include "ui/ui.hpp"
#include "ui/hookPoint.hpp"
#include "router/router.hpp"
#include "utils/crc32.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Ui::Ui()
		: JsObject(true, "[object Ui]")
	{
		jsRegisterProp("layouts", &Ui::xetter_layouts, true);
		jsRegisterProp("skins", &Ui::xetter_skins, true);
		jsRegisterProp("places", &Ui::xetter_places, true);

		jsRegisterProp("layoutProvider", &Ui::xetter_layoutProvider, false);
		jsRegisterProp("skinProvider", &Ui::xetter_skinProvider, false);
		jsRegisterProp("blockProvider", &Ui::xetter_blockProvider, false);

		jsRegisterMeth("reset", &Ui::call_reset, 0);

		jsRegisterProp("layoutName", &Ui::xetter_layoutName, false);
		jsRegisterProp("skinName", &Ui::xetter_skinName, false);

		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, es_blocks, OBJECT_TO_JSVAL(JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL)));
		jsRegisterProp("blocks", &Ui::xetter_blocks, true);

		jsRegisterProp("skin", &Ui::xetter_skin, true);

		jsRegisterMeth("print", &Ui::call_print, 0);

		jsRegisterMeth("hookPlace", &Ui::call_hookPlace, 0);
		jsRegisterMeth("hookInvoke", &Ui::call_hookInvoke, 0);
		jsRegisterMeth("hookSubscribe", &Ui::call_hookSubscribe, 0);
		jsRegisterMeth("hookUnsubscribe", &Ui::call_hookUnsubscribe, 0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	Ui::~Ui()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_skinProvider(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, es_skinProvider, vp)?true:false;
		}

		return JS_SetReservedSlot(ecx()->_jsCx, _js, es_skinProvider, *vp)?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_layoutProvider(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, es_layoutProvider, vp)?true:false;
		}

		return JS_SetReservedSlot(ecx()->_jsCx, _js, es_layoutProvider, *vp)?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_blockProvider(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, es_blockProvider, vp)?true:false;
		}

		return JS_SetReservedSlot(ecx()->_jsCx, _js, es_blockProvider, *vp)?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::call_reset(uintN argc, jsval *argv, jsval *rval)
	{
		_currentSkinPoint.reset();
		if(!JS_SetReservedSlot(ecx()->_jsCx, _js, es_skinName, JSVAL_VOID)) return false;

		_currentLayoutPoint.reset();
		if(!JS_SetReservedSlot(ecx()->_jsCx, _js, es_skinName, JSVAL_VOID)) return false;

		if(!JS_SetReservedSlot(ecx()->_jsCx, _js, es_blocks, OBJECT_TO_JSVAL(JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL)))) return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_layoutName(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_GetReservedSlot(ecx()->_jsCx, _js, es_layoutName, vp)) return false;

			if(!JSVAL_IS_STRING(*vp))
			{
				Point *stub;
				if(!currentLayout(stub)) return false;
				if(!JS_GetReservedSlot(ecx()->_jsCx, _js, es_skinName, vp)) return false;
			}
			return true;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &s))return false;
		if(!s || !s[0])
		{
			JS_ReportError(ecx()->_jsCx, "Ui.layout must be not empty");
			return false;
		}

		Path layoutPath = getPathRoot() / "layouts" / s;
		PointPtr layoutPoint = ecx()->_router->getRootPoint()->point4Path(layoutPath);
		if(!layoutPoint)
		{
			JS_ReportError(ecx()->_jsCx, "Ui.layout %s is not a path to layout", s);
			return false;
		}

		_currentLayoutPoint = layoutPoint;
		return JS_SetReservedSlot(ecx()->_jsCx, _js, es_skinName, *vp)?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_skinName(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_GetReservedSlot(ecx()->_jsCx, _js, es_skinName, vp)) return false;

			if(!JSVAL_IS_STRING(*vp))
			{
				Point *stub;
				if(!currentSkin(stub)) return false;
				if(!JS_GetReservedSlot(ecx()->_jsCx, _js, es_skinName, vp)) return false;
			}
			return true;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &s))return false;
		if(!s || !s[0])
		{
			JS_ReportError(ecx()->_jsCx, "Ui.skin must be not empty");
			return false;
		}

		Path skinPath = getPathRoot() / "skins" / s;
		PointPtr skinPoint = ecx()->_router->getRootPoint()->point4Path(skinPath);

		if(!skinPoint)
		{
			JS_ReportError(ecx()->_jsCx, "Ui.skin %s is not a path to skin", s);
			return false;
		}

		_currentSkinPoint = skinPoint;
		return JS_SetReservedSlot(ecx()->_jsCx, _js, es_skinName, *vp)?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_blocks(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, es_blocks, vp)?true:false;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_layouts(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			Path layoutsPath = getPathRoot() / "layouts";

			PointPtr layoutsPoint = ecx()->_router->getRootPoint()->point4Path(layoutsPath);
			if(!layoutsPoint)
			{
				JS_ReportError(ecx()->_jsCx, "Ui.layouts absent in structure");
				return false;
			}

			return JS_GetProperty(ecx()->_jsCx, layoutsPoint->thisJsobj(), "childs", vp)?true:false;
		}

		JS_ReportError(ecx()->_jsCx, "Ui.layouts is read only");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_skins(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			Path skinsPath = getPathRoot() / "skins";

			PointPtr skinsPoint = ecx()->_router->getRootPoint()->point4Path(skinsPath);
			if(!skinsPoint)
			{
				JS_ReportError(ecx()->_jsCx, "Ui.skins absent in structure");
				return false;
			}

			return JS_GetProperty(ecx()->_jsCx, skinsPoint->thisJsobj(), "childs", vp)?true:false;
		}

		JS_ReportError(ecx()->_jsCx, "Ui.skins is read only");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_places(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			Path placesPath = getPathRoot() / "places";

			PointPtr placesPoint = ecx()->_router->getRootPoint()->point4Path(placesPath);
			if(!placesPoint)
			{
				JS_ReportError(ecx()->_jsCx, "Ui.places absent in structure");
				return false;
			}

			return JS_GetProperty(ecx()->_jsCx, placesPoint->thisJsobj(), "childs", vp)?true:false;
		}

		JS_ReportError(ecx()->_jsCx, "Ui.places is read only");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::call_hookPlace(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1 || argc > 2)
		{
			JS_ReportError(ecx()->_jsCx, "Ui.hookPlace must be called with 1 or 2 args");
			return false;
		}

		THookId id = hookCreate(argv[0]);
		jsval data = argc>1?argv[1]:JSVAL_VOID;
		HookPointPtr h = mkp(new HookPoint(this, id, data), ROOTNAME);
		*rval = h->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::call_hookInvoke(uintN argc, jsval *argv, jsval *rval)
	{
// 		(*ecx()->_out)<<"hookp";
// 		*rval = STRING_TO_JSVAL(JS_NewStringCopyZ(ecx()->_jsCx, "hook"));
// 		return true;
		if(argc < 1 || argc > 1)
		{
			JS_ReportError(ecx()->_jsCx, "Ui.hookInvoke must be called with 1 arg");
			return false;
		}

		jsval data;
		THookId id = hookGetId(argv[0], &data);
		if(!hookInvoke(id, data, rval))
		{
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::call_hookSubscribe(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1 || argc > 3)
		{
			JS_ReportError(ecx()->_jsCx, "Ui.hookSubscribe must be called with 1-3 args");
			return false;
		}

		THookId id = hookGetId(argv[0]);
		if(!hookSubscribe(id, argc>2?JSVAL_TO_OBJECT(argv[1]):NULL, argc>2?argv[2]:argv[1], true))
		{
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::call_hookUnsubscribe(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1 || argc > 3)
		{
			JS_ReportError(ecx()->_jsCx, "Ui.hookUnsubscribe must be called with 1-3 args");
			return false;
		}

		THookId id = hookGetId(argv[0]);
		if(!hookSubscribe(id, argc>2?JSVAL_TO_OBJECT(argv[1]):NULL, argc>2?argv[2]:argv[1], false))
		{
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::xetter_skin(jsval *vp, bool isGet)
	{
		Point *skinPoint;
		if(!currentSkin(skinPoint)) return false;
		return JS_GetProperty(ecx()->_jsCx, skinPoint->thisJsobj(), "properties", vp)?true:false;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Ui::call_print(uintN argc, jsval *argv, jsval *rval)
	{
		Point *layout;
		if(!currentLayout(layout)) return false;

		jsval jsv;
		if(!layout->call_render(0, NULL, &jsv)) return false;
		return call_printHelper(jsv, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::call_printHelper(jsval val, jsval *rval)
	{
		if(JSVAL_IS_OBJECT(val))
		{
			JSObject *obj = JSVAL_TO_OBJECT(val);
			if(obj)
			{
				JSBool b;
				if(JS_HasProperty(ecx()->_jsCx, obj, "print", &b) && b)
				{
					return JS_CallFunctionName(ecx()->_jsCx, obj, "print", 0, NULL, rval)?true:false;
				}
			}
		}

		return JS_CallFunctionName(ecx()->_jsCx, ecx()->_scripter->_global, "print", 1, &val, rval)?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	Path &Ui::getPathRoot()
	{
		if(_pathRoot.empty())
		{
			//global.uiPath
			jsval prop;
			JSBool b;
			if(JS_HasProperty(ecx()->_jsCx, ecx()->_scripter->_global, "uiPath", &b) && b &&
				JS_GetProperty(ecx()->_jsCx, ecx()->_scripter->_global, "uiPath", &prop))
			{
				char *s;
				(JSExceptionReporter)JS_ConvertArguments(ecx()->_jsCx, 1, &prop, "s", &s);
				_pathRoot = s;
			}

			if(_pathRoot.empty())
			{
				_pathRoot = "/ui/";
			}
		}

		return _pathRoot;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::currentLayout(Point *&p)
	{
		if(!_currentLayoutPoint)
		{
			jsval jsv;
			if(!JS_GetReservedSlot(ecx()->_jsCx, _js, es_layoutProvider, &jsv)) return false;
			if(!JS_CallFunctionValue(ecx()->_jsCx, NULL, jsv, 0, NULL, &jsv)) return false;
			if(!xetter_layoutName(&jsv, false)) return false;
			if(!_currentLayoutPoint)
			{
				JS_ReportError(ecx()->_jsCx, "Ui.currentLayout internal error, _currentLayoutPoint is not setted");
				return false;
			}
		}

		p = _currentLayoutPoint.get();
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Ui::currentSkin(Point *&p)
	{
		if(!_currentSkinPoint)
		{
			jsval jsv;
			if(!JS_GetReservedSlot(ecx()->_jsCx, _js, es_skinProvider, &jsv)) return false;
			if(!JS_CallFunctionValue(ecx()->_jsCx, NULL, jsv, 0, NULL, &jsv)) return false;
			if(!xetter_skinName(&jsv, false)) return false;
			if(!_currentSkinPoint)
			{
				JS_ReportError(ecx()->_jsCx, "Ui.currentSkin internal error, _currentSkinPoint is not setted");
				return false;
			}
		}

		p = _currentSkinPoint.get();
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Ui::currentBlocks(const char *placeName, jsval &rval)
	{
		assert(0);
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	THookId Ui::hookCreate(jsval jsvId)
	{
		THookId id = hookGetId(jsvId, NULL, false);
		TMHooks::iterator iter = _hooks.find(id);
		if(_hooks.end() == iter)
		{
			_hooks[id];
		}

		return id;
	}

	//////////////////////////////////////////////////////////////////////////
	THookId Ui::hookGetId(jsval id, jsval *data, bool parseObj)
	{
		if(parseObj && JSVAL_IS_OBJECT(id))
		{
			JSObject *obj = JSVAL_TO_OBJECT(id);
			JSBool b;
			if(JS_HasProperty(ecx()->_jsCx, obj, "id", &b) && b)
			{
				if(!JS_GetProperty(ecx()->_jsCx, obj, "id", &id))return false;
			}

			if(JS_HasProperty(ecx()->_jsCx, obj, "data", &b) && b)
			{
				if(!JS_GetProperty(ecx()->_jsCx, obj, "data", data))return false;
			}
		}
		else
		{
			if(data)
			{
				*data = JSVAL_VOID;
			}
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &id, "s", &s))return false;
		return s;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Ui::hookInvoke(THookId id, jsval data, jsval *rval)
	{
		TMHooks::iterator iter = _hooks.find(id);
		if(_hooks.end() != iter)
		{
			if(!iter->second.invoke(id, data, rval))
			{
				if(!JS_IsExceptionPending(ecx()->_jsCx))
				{
					JS_ReportError(ecx()->_jsCx, "ui.invoke failed for hook %s", id.data());
				}
				return false;
			}

			return true;
		}

		JS_ReportError(ecx()->_jsCx, "ui.invoke for absent hook %s", id.data());
		return false;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Ui::hookSubscribe(THookId id, JSObject *obj, jsval meth, bool enable)
	{
		if(enable)
		{
			//add if absent
			_hooks[id].subscribe(obj, meth, enable);
			return true;
		}
		else
		{
			TMHooks::iterator iter = _hooks.find(id);
			if(_hooks.end() != iter)
			{
				iter->second.subscribe(obj, meth, enable);
				return true;
			}

			JS_ReportError(ecx()->_jsCx, "ui.hookUnsubscribe for absent hook %s", id.data());
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Ui::Hook::invoke(THookId id, jsval data, jsval *rval)
	{
		if(_subscribers.size())
		{
			JSContext *cx = ecx()->_jsCx;

			jsval argv[2];
			argv[0] = STRING_TO_JSVAL(JS_NewStringCopyN(cx, id.data(), id.size()));
			argv[1] = data;
			BOOST_FOREACH(TMSubscribers::value_type &v, _subscribers)
			{
				const TSubscriber &s = v.first;
				if(!JS_CallFunctionValue(ecx()->_jsCx, s.first, s.second, 2, argv, rval))
				{
					return false;
				}
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	int Ui::Hook::subscribe(JSObject *obj, jsval meth, bool enable)
	{
		TSubscriber s(obj, meth);

		TMSubscribers::iterator iter = _subscribers.find(s);

		if(enable)
		{
			if(_subscribers.end() == iter)
			{
				std::pair<TMSubscribers::iterator, bool> pb = _subscribers.insert(std::make_pair(s, 1));
				JS_AddNamedRoot(ecx()->_jsCx, (JSObject **)&pb.first->first.first, "Ui::Hook::subscribe_obj");
				JS_AddNamedRoot(ecx()->_jsCx, (jsval *)&pb.first->first.second, "Ui::Hook::subscribe_meth");
			}
			else
			{
				_subscribers[s]++;
			}
		}
		else
		{
			if(_subscribers.end() != iter)
			{
				if(iter->second <= 1)
				{
					JS_RemoveRoot(ecx()->_jsCx, (JSObject **)&iter->first.first);
					JS_RemoveRoot(ecx()->_jsCx, (jsval *)&iter->first.second);
					_subscribers.erase(iter);
				}
				else
				{
					iter->second--;
				}
			}
		}

		return _subscribers.size();
	}

}
