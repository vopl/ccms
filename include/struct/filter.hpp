#ifndef _ccms_filter_hpp_
#define _ccms_filter_hpp_

#include "scripter/jsObject.hpp"
#include "struct/filterInstance.hpp"
#include "struct/property.hpp"

namespace ccms
{
	class Point;
	class Filter
		: public JsObject
		, public Initializable
	{
		Point *_parent;
		FilterInstancePtr _instance;
		TMProperties		_properties;

	public:
		Filter(Point *parent, FilterInstancePtr instance);
		~Filter();

		virtual void init();
		virtual void deinit();
		EFilterInstanceKind getKind();
		bool process(EFilterInstanceProcessResult &fpr);

	private:
		jsval mkProperties(TMPropertyInstances &properties, TMProperties &s);
		template <class TMap> void mapDeinit(TMap &m);
	};

	typedef JsPtr<Filter> FilterPtr;
	typedef AssocSeqContainer<std::string, FilterPtr> TMFilters;
	typedef std::deque<FilterPtr> TDFilters;






	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	inline jsval Filter::mkProperties(TMPropertyInstances &properties, TMProperties &s)
	{
		JSContext *cx = ecx()->_jsCx;

		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		TMPropertyInstances::Seq::iterator iter = properties.seq.begin();
		TMPropertyInstances::Seq::iterator end = properties.seq.end();

		for(; iter!=end; iter++)
		{
			PropertyPtr property = mkp(new Property(this, iter->second), ROOTNAME);
			property->init();
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				iter->first.c_str(), 
				property->thisJsval(),
				NULL, NULL, 
				JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT);
			s.seq.push_back(s.mk(iter->first, property));
		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	template <class TMap>
	void Filter::mapDeinit(TMap &m)
	{
		typename TMap::reverse_iterator iter = m.rbegin();
		typename TMap::reverse_iterator end = m.rend();

		for(; iter!=end; iter++)
		{
			iter->second->deinit();
		}

		m.clear();
	}


}
#endif
