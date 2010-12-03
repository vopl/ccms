#include "stdafx.h"
#include "router/cache.hpp"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Cache::Cache()
		: JsObject(true, "Cache")
	{
		(JSExceptionReporter)jsRegisterMeth("set", &Cache::call_set, 2);
		(JSExceptionReporter)jsRegisterMeth("get", &Cache::call_get, 1);
		(JSExceptionReporter)jsRegisterMeth("del", &Cache::call_del, 1);
		(JSExceptionReporter)jsRegisterMeth("fire", &Cache::call_fire, 1);
	}

	//////////////////////////////////////////////////////////////////////////
	Cache::~Cache()
	{
		BOOST_FOREACH(const TMKeyValue::value_type &v, _mv)
		{
			(JSExceptionReporter)onUnset(v.right);
		}
		_me.clear();
		_mt.clear();
		_mv.clear();

	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::set(jsval key, jsval value, jsval callback, const std::vector<const char *> &delEvents, time_t timeout)
	{
		Key k(key);
		time_t now;
		time(&now);
		Value v = {now, value, callback};


		{
			del(k);
			TMKeyValue::left_iterator iter = _mv.left.insert(TMKeyValue::left_map::value_type(k, v)).first;
			if(!onSet(iter->second)) return false;
		}

		if(timeout > 0)
		{
			_mt.insert(TMKeyTimeout::value_type(k, timeout));
		}

		for(size_t i(0); i<delEvents.size(); i++)
		{
			Key ek(delEvents[i]);
			_me.insert(TMKeyEvent::value_type(k, ek));
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::get(jsval key, jsval *value, bool &present)
	{
		TMKeyValue::left_iterator iter = _mv.left.find(Key(key));
		if(_mv.left.end() == iter)
		{
			present = false;
			*value = JSVAL_VOID;
			return true;
		}

		Value v = iter->second;
		time(&v._atime);
		_mv.left.replace_data(iter, v);

		*value = v._value;
		present = true;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::del(jsval key)
	{
		del(Key(key));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::fire(jsval event)
	{
		Key ek(event);

		std::pair<TMKeyEvent::right_iterator, TMKeyEvent::right_iterator> range = _me.right.equal_range(ek);
		std::size_t amount = std::distance(range.first, range.second);

		if(amount)
		{
			std::vector<Key> keys;
			keys.reserve(amount);
			TMKeyEvent::right_iterator iter = range.first;
			for(size_t i(0); i<amount; i++)
			{
				keys.push_back(iter++ ->second);
			}

			for(size_t i(0); i<amount; i++)
			{
				del(keys[i]);
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::tick()
	{
		if(_mt.empty())
		{
			return true;
		}

		time_t now;
		time(&now);

		std::pair<TMKeyTimeout::right_iterator, TMKeyTimeout::right_iterator> range;
		range.first = _mt.right.begin();
		range.second = _mt.right.upper_bound(now);
		std::size_t amount = std::distance(range.first, range.second);

		if(amount)
		{
			std::vector<Key> keys;
			keys.reserve(amount);
			TMKeyTimeout::right_iterator iter = range.first;
			for(size_t i(0); i<amount; i++)
			{
				keys.push_back(iter++ ->second);
			}

			for(size_t i(0); i<amount; i++)
			{
				del(keys[i]);
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::delOld(time_t bound)
	{
		if(_mv.empty())
		{
			return true;
		}

		Value vBound = {bound, JSVAL_VOID};

		std::pair<TMKeyValue::right_iterator, TMKeyValue::right_iterator> range;
		range.first = _mv.right.begin();
		range.second = _mv.right.upper_bound(vBound);
		std::size_t amount = std::distance(range.first, range.second);

		if(amount)
		{
			std::vector<Key> keys;
			keys.reserve(amount);
			TMKeyValue::right_iterator iter = range.first;
			for(size_t i(0); i<amount; i++)
			{
				keys.push_back(iter++ ->second);
			}

			for(size_t i(0); i<amount; i++)
			{
				del(keys[i]);
			}
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Cache::delOld(double part)
	{
		size_t amount = _mv.size();
		if(!amount)
		{
			return true;
		}

		size_t delAmount = (size_t)(part * amount) + 1;
		if(delAmount > amount) delAmount = amount;

		std::vector<Key> keys;
		keys.reserve(delAmount);

		TMKeyValue::right_iterator iter = _mv.right.begin();
		for(size_t i(0); i<delAmount; i++)
		{
			keys.push_back(iter++ ->second);
		}

		for(size_t i(0); i<delAmount; i++)
		{
			del(keys[i]);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	size_t Cache::size()
	{
		return _mv.size();
	}


	//////////////////////////////////////////////////////////////////////////
	//cache.set(name, value, events, callback, timeout)
	bool Cache::call_set(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 2 || argc > 5)
		{
			JS_ReportError(ecx()->_jsCx, "Cache.set: must be called with 2-5 args");
			return false;
		}

		jsval jsvEvents = JSVAL_VOID;
		jsval jsvTimeout = JSVAL_VOID;
		jsval jsvCallback = JSVAL_VOID;

		if(argc > 2)
		{
			jsvEvents = argv[2];
			if(argc > 3)
			{
				jsvTimeout = argv[3];
			}
			if(argc > 4)
			{
				jsvCallback = argv[4];
			}
		}

		if(!JSVAL_IS_OBJECT(jsvEvents) && !JSVAL_IS_VOID(jsvEvents))
		{
			JS_ReportError(ecx()->_jsCx, "Cache.set: 3rd arg must be an Array of strings");
			return false;
		}
		if(JSVAL_IS_NULL(jsvEvents)) jsvEvents = JSVAL_VOID;

		if(!JSVAL_IS_OBJECT(jsvTimeout) && !JSVAL_IS_VOID(jsvTimeout))
		{
			JS_ReportError(ecx()->_jsCx, "Cache.set: 4-th arg must be an Date");
			return false;
		}
		if(JSVAL_IS_NULL(jsvTimeout)) jsvTimeout = JSVAL_VOID;

		if(!JSVAL_IS_OBJECT(jsvCallback) && !JSVAL_IS_VOID(jsvCallback))
		{
			JS_ReportError(ecx()->_jsCx, "Cache.set: 5-rd arg must be an function");
			return false;
		}
		if(JSVAL_IS_NULL(jsvCallback)) jsvCallback = JSVAL_VOID;



		std::vector<const char *> events;
		if(JSVAL_IS_OBJECT(jsvEvents))
		{
			JSObject *eventsArray = JSVAL_TO_OBJECT(jsvEvents);
			if(!JS_IsArrayObject(ecx()->_jsCx, eventsArray))
			{
				JS_ReportError(ecx()->_jsCx, "Cache.set: 3-rd arg must be an array of strings");
				return false;
			}

			jsuint len;
			if(!JS_GetArrayLength(ecx()->_jsCx, eventsArray, &len)) return false;

			events.resize(len);
			for(jsuint i(0); i<len; i++)
			{
				jsval jsv;
				if(!JS_GetElement(ecx()->_jsCx, eventsArray, i, &jsv)) return false;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &events[i])) return false;
			}
		}

		time_t timeout(0);
		if(JSVAL_IS_OBJECT(jsvTimeout))
		{
			JSObject *obj = JSVAL_TO_OBJECT(jsvTimeout);
			JSClass *cls = JS_GET_CLASS(cx, obj);
			if(strcmp("Date", cls->name))
			{
				JS_ReportError(ecx()->_jsCx, "Cache.set: 4-th arg must be an Date object");
				return false;
			}

			jsval jsv;
			if(!JS_CallFunctionName(ecx()->_jsCx, obj, "getTime", 0, NULL, &jsv)) return false;
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "d", &d))return false;
			timeout = (time_t)(d/1000);
		}

		if(JSVAL_IS_OBJECT(jsvCallback))
		{
			if(JSTYPE_FUNCTION != JS_TypeOfValue(ecx()->_jsCx, jsvCallback))
			{
				JS_ReportError(ecx()->_jsCx, "Cache.set: 5-rd arg must be an Function");
				return false;
			}
		}


		*rval = JSVAL_VOID;
		return set(argv[0], argv[1], jsvCallback, events, timeout);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::call_get(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 1)
		{
			JS_ReportError(ecx()->_jsCx, "Cache.get: must be called with 1 arg");
			return false;
		}

		bool present;
		return get(argv[0], rval, present);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::call_del(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 1)
		{
			JS_ReportError(ecx()->_jsCx, "Cache.del: must be called with 1 arg");
			return false;
		}
		*rval = JSVAL_VOID;
		return del(argv[0]);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::call_fire(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 1)
		{
			JS_ReportError(ecx()->_jsCx, "Cache.fire: must be called with 1 arg");
			return false;
		}
		*rval = JSVAL_VOID;
		return fire(argv[0]);
	}


	//////////////////////////////////////////////////////////////////////////
	bool Cache::del(const Key &k)
	{
		TMKeyValue::left_iterator iter = _mv.left.find(k);
		if(_mv.left.end() != iter)
		{
			if(!onUnset(iter->second)) return false;
			_mv.left.erase(iter);
			_mt.left.erase(k);
			_me.left.erase(k);
			return true;
		}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Cache::onSet(const Value &v)
	{
		if(JSVAL_IS_GCTHING(v._value))
			(JSExceptionReporter)JS_AddRoot(ecx()->_jsCx, (void *)&v._value);
		if(JSVAL_IS_GCTHING(v._callback))
			(JSExceptionReporter)JS_AddRoot(ecx()->_jsCx, (void *)&v._callback);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::onUnset(const Value &v)
	{
		if(JSVAL_IS_GCTHING(v._callback))
		{
			if(JSVAL_IS_OBJECT(v._callback))
			{
				jsval jsv;
				(JSExceptionReporter)JS_CallFunctionValue(ecx()->_jsCx, NULL, v._callback, 1, (jsval *)&v._value, &jsv);
			}
			(JSExceptionReporter)JS_RemoveRoot(ecx()->_jsCx, (void *)&v._callback);
		}

		if(JSVAL_IS_GCTHING(v._value))
			(JSExceptionReporter)JS_RemoveRoot(ecx()->_jsCx, (void *)&v._value);
		return true;
	}








	//////////////////////////////////////////////////////////////////////////
	Cache::Key::Key()
		: _kd(NULL)
	{
	}



	//////////////////////////////////////////////////////////////////////////
	Cache::Key::Key(jsval jsv)
	{
		const char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &s))
		{
			(JSExceptionReporter)false;
			s = "stub";
		}
		size_t dataSize = strlen(s);
		_kd = (KeyData *)malloc(sizeof(KeyData) + dataSize);
		_kd->_refCount = 1;
		_kd->_size = dataSize;
		memcpy(_kd->_data, s, dataSize);
		_kd->_data[dataSize] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	Cache::Key::Key(const char *s)
	{
		size_t dataSize = strlen(s);
		_kd = (KeyData *)malloc(sizeof(KeyData) + dataSize);
		_kd->_refCount = 1;
		_kd->_size = dataSize;
		memcpy(_kd->_data, s, dataSize);
		_kd->_data[dataSize] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	Cache::Key::Key(const Key &from)
		: _kd(from._kd)
	{
		_kd->_refCount++;
	}

	//////////////////////////////////////////////////////////////////////////
	Cache::Key::~Key()
	{
		if(_kd)
		{
			if(!--_kd->_refCount)
			{
				free(_kd);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Cache::Key::operator=(const Key &from)
	{
		if(_kd)
		{
			if(!--_kd->_refCount)
			{
				free(_kd);
			}
		}

		_kd = from._kd;
		if(_kd)
		{
			_kd->_refCount++;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cache::Key::operator<(const Key &with) const
	{
		if(_kd->_size < with._kd->_size) return true;
		if(_kd->_size > with._kd->_size) return false;
		return memcmp(_kd->_data, with._kd->_data, _kd->_size)<0;
	}



}

