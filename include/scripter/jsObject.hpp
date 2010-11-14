#ifndef _ccms_jsObject_h_
#define _ccms_jsObject_h_

#include <js/jsapi.h>
#include "router/execContext.hpp"
#include "scripter/exceptionReporter.hpp"
#include "scripter/utils.hpp"

#include "scripter/jsPtr.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	struct BaseDelegatXetter
	{
		virtual bool act(JSObject *obj, jsval *vp, bool isGet)=0;
	};
	typedef boost::shared_ptr<BaseDelegatXetter> BaseDelegatXetterPtr;

	//////////////////////////////////////////////////////////////////////////
	struct BaseDelegatCall
	{
		virtual bool act(JSObject *obj, uintN argc, jsval *argv, jsval *rval)=0;
	};
	typedef boost::shared_ptr<BaseDelegatCall> BaseDelegatCallPtr;

	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	struct DelegatXetter
		: BaseDelegatXetter
	{
		typedef bool (Target::*TXetter)(jsval *vp, bool isGet);
		Target *_target;
		TXetter _xetter;
		DelegatXetter(Target *target, TXetter xetter)
			: _target(target)
			, _xetter(xetter)
		{
		}
		virtual bool act(JSObject *obj, jsval *vp, bool isGet)
		{
			return (_target->*_xetter)(vp, isGet);
		}
	};

	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	struct DelegatCall
		: BaseDelegatCall
	{
		typedef bool (Target::*TCall)(uintN argc, jsval *argv, jsval *rval);
		Target *_target;
		TCall _call;
		DelegatCall(Target *target, TCall call)
			: _target(target)
			, _call(call)
		{
		}
		virtual bool act(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			return (_target->*_call)(argc, argv, rval);
		}
	};


	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	struct DelegatCallObj
		: BaseDelegatCall
	{
		typedef bool (Target::*TCall)(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		Target *_target;
		TCall _call;
		DelegatCallObj(Target *target, TCall call)
			: _target(target)
			, _call(call)
		{
		}
		virtual bool act(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
		{
			return (_target->*_call)(obj, argc, argv, rval);
		}
	};



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class JsObject;
	typedef JsPtr<JsObject> JsObjectPtr;
	//////////////////////////////////////////////////////////////////////////
	class JsObject
	{
	public:
		static bool initClass();
		static JSBool _cppObjectGetterClass(JSContext *cx, JSObject *obj, jsid id, jsval *vp);
		static JSBool _cppObjectCtor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

		//управление временем жизни
		template<class T> static JsPtr<T> mkp(JSObject *p, const char *file, size_t line);
		template<class T> static JsPtr<T> mkp(JsObject *p, const char *file, size_t line);
		template<class T> static JsPtr<T> mkp(T *p, const char *file, size_t line);


		//управление временем жизни
		static void deleterJs(JSContext *cx, JSObject *obj);
		virtual void vdeleterJs()
		{
			delete this;
		}
		static JSBool propertyOp_add(JSContext *cx, JSObject *obj, jsid id, jsval *vp);
		static JSBool propertyOp_del(JSContext *cx, JSObject *obj, jsid id, jsval *vp);
		static JSBool propertyOp_set(JSContext *cx, JSObject *obj, jsid id, jsval *vp);
		static JSBool propertyOp_get(JSContext *cx, JSObject *obj, jsid id, jsval *vp);
		static JSBool convertOp(JSContext *cx, JSObject *obj, JSType type, jsval *vp);
		static JSBool enumerateOp(JSContext *cx, JSObject *obj);

		static JSBool class_call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		static JSBool class_construct(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

		virtual JSBool onPropAdd(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropDel(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropSet(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropGet(JSObject *obj, jsid id, jsval *vp);

		virtual JSBool onEnumerate();

		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);


	public:
		template <class Target>
		bool jsRegisterProp(
			const char *name,
			bool (Target::*xetter)(jsval *vp, bool isGet),
			bool readOnly=true);

		bool jsRegisterProp(
			const char *name,
			const std::string &value,
			bool readOnly=true);

		bool jsRegisterProp(
			const char *name,
			jsval value,
			bool readOnly=true);

		bool jsRegisterProp(
			const char *name,
			JSObject *value,
			bool readOnly=true);

		template <class Target>
		bool jsRegisterMeth(
			const char *name,
			bool (Target::*call)(uintN argc, jsval *argv, jsval *rval),
			size_t argc);

		template <class Target>
		bool jsRegisterMeth(
			const char *name,
			bool (Target::*call)(JSObject *obj, uintN argc, jsval *argv, jsval *rval),
			size_t argc);

	public:
		static JsObject *thisObj(JSObject *js, JSObject *callee=NULL);
		static JsObject *thisObjWithProp(JSObject *js, jsid id);
		jsval thisJsval();
		JSObject *&thisJsobj();

		//void setPrototype(JsPtr proto);

		//улита смотрит если не void то изымает из протекции сборщика мусора
		static void clean2void(jsval &jsv);
		//утила для удаления пропертей с флагом permanent
		void deletePermanent(const char *name);
		//утила для получения проперти с заданным именем
		jsval getProperty(const char *name);
		JSObject *getPropertyJSObject(const char *name);
		JsObject *getPropertyJsObject(const char *name);
		template<class T>
		JsPtr<T> getPropertyPtr(const char *name);

	protected:
		static JSBool _xetter(JSContext *cx, JSObject *obj, jsid id, jsval *vp, bool isGet);
		static JSBool _getter(JSContext *cx, JSObject *obj, jsid id, jsval *vp);
		static JSBool _setter(JSContext *cx, JSObject *obj, jsid id, jsval *vp);
		static JSBool _xall(JSContext *cx, uintN argc, jsval *vp);
		static JSBool _callFast(JSContext *cx, uintN argc, jsval *vp);
		

		static JSBool _call_toString(JSContext *cx, uintN argc, jsval *vp);

		virtual JSBool _xetter4unknown(JSContext *cx, JSObject *js, jsid id, jsval *val, bool isGet);
		virtual JSBool _xall4unknown(JSContext *cx, uintN argc, jsval *vp);


	protected:
		JSObject *_js;
		//JsPtr _prototype;

		std::string	_strVal;




	private:
#if USE_JSOBJECT_ROOTNAME
#	define ROOTNAME __FILE__, __LINE__
		template<class T> friend class ccms::JsPtr;
		std::string	_rootName;
#else
#	define ROOTNAME NULL, 0
#endif
		//кзетеры этого объекта
		//typedef std::map<JsidProxy, BaseDelegatXetterPtr> TMXetters;
		typedef std::map<jsid, BaseDelegatXetterPtr> TMXetters;
		TMXetters _xetters;
		//вызовы этого объекта
		typedef std::map<JSFunction *, BaseDelegatCallPtr> TMCalls;
		TMCalls _calls;

	private:
		size_t _useCounter;
	public:
		JsObject(bool doJsCreate = true, const char *strVal=NULL);
		virtual ~JsObject();

		virtual void jsCreate();
		virtual void jsDestroy();

		size_t incRef();
		size_t decRef();
	};



	//////////////////////////////////////////////////////////////////////////
	template<class T>
	JsPtr<T> JsObject::mkp(JSObject *p, const char *file, size_t line)
	{
		return mkp(dynamic_cast<T *>(thisObj(p)), file, line);
	}

	//////////////////////////////////////////////////////////////////////////
	template<class T>
	JsPtr<T> JsObject::mkp(JsObject *p, const char *file, size_t line)
	{
		return mkp(dynamic_cast<T *>(p), file, line);
	}

	//////////////////////////////////////////////////////////////////////////
	template<class T>
	JsPtr<T> JsObject::mkp(T *p, const char *file, size_t line)
	{
		if(!p)
		{
			return JsPtr<T>();
		}

#if USE_JSOBJECT_ROOTNAME
		if(p->_rootName.empty())
		{
				char tmp[1024];
				sprintf(tmp, "%s, %s, %u", typeid(*p).name(), file, line);
				p->_rootName = tmp;
		}
#endif
		return JsPtr<T>(p);
	}


	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	bool JsObject::jsRegisterProp(
		const char *name,
		bool (Target::*xetter)(jsval *vp, bool isGet),
		bool readOnly)
	{
		JSBool ok = JS_DefineProperty(
			ecx()->_jsCx, 
			_js, 
			name, 
			JSVAL_ZERO, 
			&_getter, 
			&_setter, 
			JSPROP_ENUMERATE|
			JSPROP_PERMANENT|
			JSPROP_SHARED|
			(readOnly?JSPROP_READONLY:0));

		if(!ok)
		{
			return false;
		}
		jsid jid;
		JSString *s = JS_InternString(ecx()->_jsCx, name);
		if(!JS_ValueToId(ecx()->_jsCx, STRING_TO_JSVAL(s), &jid))return false;
		_xetters[jid].reset(new DelegatXetter<Target>((Target *)this, xetter));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	bool JsObject::jsRegisterMeth(
		const char *name,
		bool (Target::*call)(uintN argc, jsval *argv, jsval *rval),
		size_t argc)
	{

		JSFunction *f = JS_DefineFunction(
			ecx()->_jsCx, 
			_js, 
			name, 
 			(JSNative)&_callFast, 
			argc,
			JSFUN_FAST_NATIVE|
			JSPROP_ENUMERATE|
			JSPROP_PERMANENT|
			JSPROP_READONLY);

		if(!f)
		{
			return false;
		}
		_calls[f].reset(new DelegatCall<Target>((Target *)this, call));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	bool JsObject::jsRegisterMeth(
		const char *name,
		bool (Target::*call)(JSObject *obj, uintN argc, jsval *argv, jsval *rval),
		size_t argc)
	{

		JSFunction *f = JS_DefineFunction(
			ecx()->_jsCx, 
			_js, 
			name, 
			(JSNative)&_callFast, 
			argc,
			JSFUN_FAST_NATIVE|
			JSPROP_ENUMERATE|
			JSPROP_PERMANENT|
			JSPROP_READONLY);

		if(!f)
		{
			return false;
		}
		_calls[f].reset(new DelegatCallObj<Target>((Target *)this, call));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	inline void JsObject::clean2void(jsval &jsv)
	{
		if(!JSVAL_IS_VOID(jsv))
		{
			(JSExceptionReporter)JS_RemoveRoot(ecx()->_jsCx, &jsv);
			jsv = JSVAL_VOID;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	inline void JsObject::deletePermanent(const char *name)
	{
		JSBool b = JS_FALSE;
		(JSExceptionReporter)JS_SetPropertyAttributes(ecx()->_jsCx, _js, name, 0, &b);
		if(b)
		{
			(JSExceptionReporter)JS_DeleteProperty(ecx()->_jsCx, _js, name);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	inline jsval JsObject::getProperty(const char *name)
	{
		jsval res = JSVAL_VOID;
		(JSExceptionReporter)JS_GetProperty(ecx()->_jsCx, _js, name, &res);
		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	inline JSObject *JsObject::getPropertyJSObject(const char *name)
	{
		jsval v = getProperty(name);
		if(JSVAL_IS_OBJECT(v))
		{
			return JSVAL_TO_OBJECT(v);
		}
		return NULL;
	}
	
	//////////////////////////////////////////////////////////////////////////
	inline JsObject *JsObject::getPropertyJsObject(const char *name)
	{
		return thisObj(getPropertyJSObject(name));
	}
	
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	JsPtr<T> JsObject::getPropertyPtr(const char *name)
	{
		return mkp(dynamic_cast<T *>(getPropertyJsObject(name)), ROOTNAME);
	}


}

#endif

