#ifndef _ccms_newPropsCollector_hpp_
#define _ccms_newPropsCollector_hpp_

#include "scripter/utils.hpp"

namespace ccms
{
	class NewPropsCollector
	{
		//typedef std::set<JsidProxy> TSProp;
		typedef std::set<jsid> TSProp;
		typedef std::map<JSObject *, TSProp> TMObj;
		TMObj _objects;
	public:
		void set(JSContext *cx, JSObject *obj, jsid id)
		{
			_objects[obj].insert(id);
		}
		void reset(JSContext *cx, JSObject *obj)
		{
			_objects.erase(obj);
		}

		void deleteAll()
		{
			JSContext *cx = ecx()->_jsCx;

			TMObj::iterator oiter=_objects.begin();
			TMObj::iterator oend=_objects.end();
			for(; oiter!=oend; oiter++)
			{
				TSProp::iterator piter=oiter->second.begin();
				TSProp::iterator pend=oiter->second.end();
				for(; piter!=pend; piter++)
				{
					(JSExceptionReporter)JS_DeletePropertyById(cx, oiter->first, *piter);
				}
			}

			_objects.clear();
		}
	};
}
#endif
