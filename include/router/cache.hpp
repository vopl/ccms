#ifndef _ccms_cache_hpp_
#define _ccms_cache_hpp_

#include "scripter/jsObject.hpp"

#include <boost/bimap/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>

namespace ccms
{
	class Cache
		: public JsObject
	{
	public:
		Cache();
		~Cache();

		bool set(jsval key, jsval value, const std::vector<const char *> &delEvents, time_t timeout);
		bool get(jsval key, jsval *value, bool &present);
		bool del(jsval key);

		bool fire(jsval event);
		bool tick();
		bool delOld(time_t bound);
		bool delOld(double part);

		size_t size();

		bool call_set(uintN argc, jsval *argv, jsval *rval);
		bool call_get(uintN argc, jsval *argv, jsval *rval);
		bool call_del(uintN argc, jsval *argv, jsval *rval);
		bool call_fire(uintN argc, jsval *argv, jsval *rval);

	private:
		class Key
		{
			struct KeyData
			{
				size_t _refCount;
				size_t _size;
				char _data[1];
			};
			KeyData *_kd;

		public:
			Key();
			Key(jsval jsv);
			Key(const char *csz);
			Key(const Key &);
			~Key();

			void operator=(const Key &);
			bool operator<(const Key &) const;
		};

		struct Value
		{
			time_t	_atime;
			jsval	_value;

			bool operator<(const Value &with) const {return _atime < with._atime;}
		};

	private:
		bool del(const Key &k);
		bool protectGc(const jsval &jsv);
		bool unprotectGc(const jsval &jsv);

	private:
		typedef boost::bimaps::bimap<
			boost::bimaps::multiset_of<Key>, 
			boost::bimaps::multiset_of<Key>
		> TMKeyEvent;

		typedef boost::bimaps::bimap<
			boost::bimaps::set_of<Key>,
			boost::bimaps::multiset_of<time_t>
		> TMKeyTimeout;


		typedef boost::bimaps::bimap<
			boost::bimaps::set_of<Key>,
			boost::bimaps::multiset_of<Value>
		> TMKeyValue;

		TMKeyEvent		_me;
		TMKeyTimeout	_mt;
		TMKeyValue		_mv;
	};
	typedef JsPtr<Cache> CachePtr;

}

#endif
