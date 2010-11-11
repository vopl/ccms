#include "stdafx.h"
#include "scripter/jsObject.hpp"
#include "router/execContext.hpp"
#include "scripter/newPropsCollector.hpp"
#include "scripter/profiler.hpp"

namespace ccms
{
	static JSClass jsObject_class = {
		"CppObject", JSCLASS_HAS_PRIVATE|JSCLASS_HAS_RESERVED_SLOTS(32),
		&JsObject::propertyOp_add, &JsObject::propertyOp_del, &JsObject::propertyOp_get, &JsObject::propertyOp_set,
		&JsObject::enumerateOp, JS_ResolveStub, &JsObject::convertOp, &JsObject::deleterJs,
		NULL, // JSGetObjectOps      getObjectOps;
		NULL, // JSCheckAccessOp     checkAccess;
		&JsObject::class_call, // JSNative            call;
		&JsObject::class_construct, // JSNative            construct;
		NULL, // JSXDRObjectOp       xdrObject;
		NULL, // JSHasInstanceOp     hasInstance;
		NULL, // JSMarkOp            mark;
		NULL, // JSReserveSlotsOp    reserveSlots;
	};

	//////////////////////////////////////////////////////////////////////////
	void JsObject::deleterJs(JSContext *cx, JSObject *obj)
	{
		ecx()->_npc->reset(cx, obj);
		thisObj(obj)->vdeleterJs();
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::propertyOp_add(JSContext *cx, JSObject *obj, jsid id, jsval *vp)
	{
		JsObject *this_ = thisObj(obj);
		if(this_)
		{
			return this_->onPropAdd(obj, id, vp);
		}
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::propertyOp_del(JSContext *cx, JSObject *obj, jsid id, jsval *vp)
	{
		JsObject *this_ = thisObj(obj);
		if(this_)
		{
			return this_->onPropDel(obj, id, vp);
		}
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::propertyOp_set(JSContext *cx, JSObject *obj, jsid id, jsval *vp)
	{
		JsObject *this_ = thisObj(obj);
		if(this_)
		{
			return this_->onPropSet(obj, id, vp);
		}
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::propertyOp_get(JSContext *cx, JSObject *obj, jsid id, jsval *vp)
	{
		JsObject *this_ = thisObj(obj);
		if(this_)
		{
			return this_->onPropGet(obj, id, vp);
		}
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::convertOp(JSContext *cx, JSObject *obj, JSType type, jsval *vp)
	{
		JsObject *this_ = thisObj(obj);
		if(this_)
		{
			*vp = STRING_TO_JSVAL(JS_NewStringCopyN(cx, this_->_strVal.c_str(), this_->_strVal.size()));
		}
		else
		{
			//для объекта, у которого this_ как прототип
			*vp = JSVAL_VOID;
		}
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::enumerateOp(JSContext *cx, JSObject *obj)
	{
		return thisObj(obj)->onEnumerate();
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::class_call(JSContext *cx, JSObject *obj_, uintN argc, jsval *argv, jsval *rval)
	{
		JSObject *obj = JSVAL_TO_OBJECT(JS_ARGV_CALLEE(argv));
		JsObject *this_ = thisObj(obj);
		if(!this_)
		{
			JS_ReportError(cx, "[JsObject.call failed to determine this object]");
			return JS_FALSE;
		}
		//////////////////////////////////////////////////////////////////////////
#if USE_PROFILER
		std::string name = typeid(*this_).name()+6;
		name += ".call";
		ProfilerScopeHelper psh(g_profiler, NULL, name.data());
#endif
		//////////////////////////////////////////////////////////////////////////
		return this_->onClassCall(obj_, argc, argv, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::class_construct(JSContext *cx, JSObject *obj_, uintN argc, jsval *argv, jsval *rval)
	{
		JSObject *obj = JSVAL_TO_OBJECT(JS_ARGV_CALLEE(argv));
		JsObject *this_ = thisObj(obj);
		if(!this_)
		{
			JS_ReportError(cx, "[JsObject.construct failed to determine this object]");
			return JS_FALSE;
		}
		//////////////////////////////////////////////////////////////////////////
#if USE_PROFILER
		std::string name = typeid(*this_).name()+6;
		name += ".construct";
		ProfilerScopeHelper psh(g_profiler, NULL, name.data());
#endif
		//////////////////////////////////////////////////////////////////////////
		return this_->onClassConstruct(obj_, argc, argv, rval);
	}



	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::onPropAdd(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::onPropDel(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::onPropSet(JSObject *obj, jsid id, jsval *vp)
	{
		ecx()->_npc->set(ecx()->_jsCx, thisJsobj(), id);
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::onPropGet(JSObject *obj, jsid id, jsval *vp)
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::onEnumerate()
	{
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		JS_ReportError(ecx()->_jsCx, "[JsObject.onClassCall is not implemented]");
		return JS_FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		JS_ReportError(ecx()->_jsCx, "[JsObject.onClassConstruct is not implemented]");
		return JS_FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	bool JsObject::jsRegisterProp(
		const char *name,
		const std::string &value,
		bool readOnly)
	{
		JSString *str = JS_NewStringCopyN(ecx()->_jsCx, value.c_str(), value.size());
		if(!str)
		{
			return false;
		}
		return jsRegisterProp(name, STRING_TO_JSVAL(str), readOnly);
	}

	//////////////////////////////////////////////////////////////////////////
	bool JsObject::jsRegisterProp(
		const char *name,
		jsval value,
		bool readOnly)
	{
		JSBool ok = JS_DefineProperty(
			ecx()->_jsCx, 
			_js, 
			name, 
			value, 
			NULL, NULL, 
			JSPROP_ENUMERATE|
			JSPROP_PERMANENT|
			(readOnly?JSPROP_READONLY:0));
		return ok?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool JsObject::jsRegisterProp(
		const char *name,
		JSObject *value,
		bool readOnly)
	{
		JSBool ok = JS_DefineProperty(
			ecx()->_jsCx, 
			_js, 
			name, 
			OBJECT_TO_JSVAL(value), 
			NULL, NULL, 
			JSPROP_ENUMERATE|
			JSPROP_PERMANENT|
			(readOnly?JSPROP_READONLY:0));
		return ok?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	//static std::map<std::string, std::set<JsObject *> > _objects;
	JsObject::JsObject(bool doJsCreate, const char *strVal)
		: _js(NULL)
		, _strVal(strVal?strVal:"[object CppObject]")
		, _useCounter(0)
	{
		//_objects[strVal].insert(this);
		if(doJsCreate)
		{
			jsCreate();
		}
	}

	JsObject::~JsObject()
	{
		//_objects[_strVal].erase(this);
	}

	//////////////////////////////////////////////////////////////////////////
	void JsObject::jsCreate()
	{
		if(_js)
		{
			return;
		}

		_js = JS_NewObject(ecx()->_jsCx, &jsObject_class, NULL, NULL);
		if(!JS_SetPrivate(ecx()->_jsCx, _js, this))
		{
			assert(0);
			if(JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportPendingException(ecx()->_jsCx);
				JS_ClearPendingException(ecx()->_jsCx);
				return;
			}
		}

		JSFunction *f = JS_DefineFunction(
			ecx()->_jsCx, 
			_js, 
			"toString", 
			(JSNative)&_call_toString, 
			0,
			JSFUN_FAST_NATIVE|
			//JSPROP_ENUMERATE|
			JSPROP_PERMANENT|
			JSPROP_SHARED|
			0);

		assert(f);
	}

	//////////////////////////////////////////////////////////////////////////
	void JsObject::jsDestroy()
	{
		if(!_js)
		{
			return;
		}

		if(_js)
		{
			_js = NULL;
		}
		_calls.clear();
		_xetters.clear();
	}

	
	//////////////////////////////////////////////////////////////////////////
	jsval JsObject::thisJsval()
	{
		return OBJECT_TO_JSVAL(_js);
	}
	
	//////////////////////////////////////////////////////////////////////////
	JSObject *&JsObject::thisJsobj()
	{
		return _js;
	}

	//////////////////////////////////////////////////////////////////////////
// 	void JsObject::setPrototype(JsPtr proto)
// 	{
// 		_prototype = proto;
// 		if(_prototype)
// 		{
// 			JS_SetPrototype(ecx()->_jsCx, thisJsobj(), _prototype->thisJsobj());
// 		}
// 		else
// 		{
// 			JS_SetPrototype(ecx()->_jsCx, thisJsobj(), NULL);
// 		}
// 	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::_xetter4unknown(JSContext *cx, JSObject *js, jsid id, jsval *val, bool isGet)
	{
		assert(0);
		return JS_TRUE;

// 		if(_prototype)
// 		{
// 			return JsObject::_xetter(cx, _prototype->thisJsobj(), id, val, isGet);
// 		}

// 		char *s;
// 		if(!JS_ConvertArguments(cx, 1, &id, "s", &s))return false;
// 		JS_ReportError(cx, "[JsObject.xetter unable to find property '%s']", s);
// 		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::_xall4unknown(JSContext *cx, uintN argc, jsval *vp)
	{
		assert(0);
		return JS_TRUE;

// 		if(_prototype)
// 		{
// 			return JsObject::_xall(cx, _prototype->thisJsobj(), argc, vp);
// 		}

// 		JSObject *funobj = JSVAL_TO_OBJECT(JS_ARGV_CALLEE(argv));
// 		JSFunction *func = (JSFunction *) JS_GetPrivate(cx, funobj);
// 
// 		JSString *str = JS_GetFunctionId(func);
// 		jsval jsvs = STRING_TO_JSVAL(str);
// 		char *s;
// 		if(!JS_ConvertArguments(cx, 1, &jsvs, "s", &s))return false;
// 		JS_ReportError(cx, "[JsObject.xall unable to find meth '%s']", s);
// 		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::_xetter(JSContext *cx, JSObject *js, jsid id, jsval *val, bool isGet)
	{
		JsObject *this_ = thisObjWithProp(js, id);

		if(!this_)
		{
			char *s = NULL;
			jsval jsv;
			(JSExceptionReporter)JS_IdToValue(cx, id, &jsv);
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
			JS_ReportError(cx, "JsObject::_xetter unknown this for property '%s'", s);
			return JS_FALSE;
		}

		//////////////////////////////////////////////////////////////////////////
#if USE_PROFILER
		char *s;
		jsval id2 = id;
		JS_ConvertArguments(cx, 1, &id2, "s", &s);
		std::string name = typeid(*this_).name()+6;
		name += ".";
		name += isGet?"get_":"set_";
		name += s;
		ProfilerScopeHelper psh(g_profiler, NULL, name.data());
#endif
		//////////////////////////////////////////////////////////////////////////

		//assert(this_->_js == js);// js может быть объектом, для которого этот - прототип
		assert(ecx()->_jsCx ==cx);

		TMXetters::iterator iter = this_->_xetters.find(id);

		if(this_->_xetters.end() == iter)
		{
			return this_->_xetter4unknown(cx, js, id, val, isGet);
		}

		if(iter->second->act(js, val, isGet))
		{
			return JS_TRUE;
		}

		return JS_FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::_getter(JSContext *cx, JSObject *js, jsid id, jsval *val)
	{
		return _xetter(cx, js, id, val, true);
	}
	
	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::_setter(JSContext *cx, JSObject *js, jsid id, jsval *val)
	{
		return _xetter(cx, js, id, val, false);
	}
	
	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::_xall(JSContext *cx, uintN argc, jsval *vp)
	{
		JSObject *funobj = JSVAL_TO_OBJECT(JS_CALLEE(cx, vp));
		JsObject *this_ = thisObj(JSVAL_TO_OBJECT(JS_THIS(cx,vp)), funobj);
		//assert(this_->_js == js); js может быть объектом, для которого этот - прототип
		assert(ecx()->_jsCx ==cx);


		JSFunction *func = (JSFunction *) JS_GetPrivate(cx, funobj);

		if(!func)
		{
			JS_ReportError(cx, "[JsObject.xall must be called with a function (internal error?)]");
			return false;
		}

		//////////////////////////////////////////////////////////////////////////
#if USE_PROFILER
		std::string name = typeid(*this_).name()+6;
		name += ".call_";
		name += JS_GetFunctionName(func);
		ProfilerScopeHelper psh(g_profiler, NULL, name.data());
#endif
		//////////////////////////////////////////////////////////////////////////

		TMCalls::iterator iter = this_->_calls.find(func);
		if(this_->_calls.end() == iter)
		{
			return this_->_xall4unknown(cx, argc, vp);
		}

		if(iter->second->act(JSVAL_TO_OBJECT(JS_THIS(cx,vp)), argc, JS_ARGV(cx, vp), &JS_RVAL(cx, vp)))
		{
			return JS_TRUE;
		}

		//JS_ReportError(cx, "[JsObject.xall actor return false]");
		return JS_FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::_callFast(JSContext *cx, uintN argc, jsval *vp)
	{
		return _xall(cx, argc, vp);
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool JsObject::_call_toString(JSContext *cx, uintN argc, jsval *vp)
	{
		JsObject *this_ = JsObject::thisObj(JS_THIS_OBJECT(cx, vp), JSVAL_TO_OBJECT(JS_CALLEE(cx, vp)));
		if(this_)
		{
			//////////////////////////////////////////////////////////////////////////
#if USE_PROFILER
			std::string name = typeid(*this_).name()+6;
			name += ".call_toString";
			ProfilerScopeHelper psh(g_profiler, NULL, name.data());
#endif
			//////////////////////////////////////////////////////////////////////////

			*vp = STRING_TO_JSVAL(JS_NewStringCopyN(cx, this_->_strVal.c_str(), this_->_strVal.size()));
			return JS_TRUE;
		}

		assert(0);
		return JS_FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	JsObject *JsObject::thisObj(JSObject *js, JSObject *callee)
	{
		if(callee)
		{
			js = JS_GetParent(ecx()->_jsCx, callee);
		}

		if(!js)
		{
			return NULL;
		}

		JSClass *cls = JS_GetClass(js);
		if(cls == &jsObject_class)
		//if(cls->flags & JSCLASS_HAS_PRIVATE)
		{
			JsObject *this_ = (JsObject *)JS_GetPrivate(ecx()->_jsCx, js);
			assert(this_->_js == js || !this_->_js);
			return this_;
		}

		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	JsObject *JsObject::thisObjWithProp(JSObject *js, jsid id)
	{
		while(js)
		{
			JSClass *cls = JS_GetClass(js);
			if(cls == &jsObject_class)
			{
				JSBool b = JS_FALSE;
				(JSExceptionReporter)JS_AlreadyHasOwnPropertyById(ecx()->_jsCx,js, id, &b);

				if(b)
				{
					JsObject *this_ = (JsObject *)JS_GetPrivate(ecx()->_jsCx, js);
					return this_;
				}
			}

			js = JS_GetPrototype(ecx()->_jsCx, js);
		}
		return NULL;
	}


	//////////////////////////////////////////////////////////////////////////
	size_t JsObject::incRef()
	{
		assert(_js);
		if(!_useCounter)
		{
#if USE_JSOBJECT_ROOTNAME
			JS_AddNamedRoot(ecx()->_jsCx, &_js, _rootName.data());
#else
			JS_AddNamedRoot(ecx()->_jsCx, &_js, typeid(*this).name());
#endif
		}
		_useCounter++;
		return _useCounter;
	}
	
	//////////////////////////////////////////////////////////////////////////
	size_t JsObject::decRef()
	{
		assert(_js);
		assert(_useCounter);

		_useCounter--;
		if(!_useCounter)
		{
			JS_RemoveRoot(ecx()->_jsCx, &_js);
		}
		return _useCounter;
	}

}

