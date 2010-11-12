#ifndef _ccms_point_hpp_
#define _ccms_point_hpp_

#include "struct/pointInstance.hpp"
#include "struct/filter.hpp"
#include "struct/property.hpp"
#include "struct/static.hpp"

namespace ccms
{
	class Point;
	typedef JsPtr<Point> PointPtr;
	typedef AssocSeqContainer<std::string, PointPtr> TMPoints;

	//////////////////////////////////////////////////////////////////////////
	class Point
		: public JsObject
		, public Initializable
	{
	public:
		Point(
			Point *parent,
			PointInstancePtr instance, 
			PointInstancePtr superInstance, 
			const Path &path);
		~Point();

		virtual void init();
		virtual void deinit();

		void structChanged(const Path &where);

		const Path & getPath();
		const std::string getId(const char *prefix=NULL);

		PointPtr point4Path(const Path &path); 

		bool executeFilters(EFilterInstanceKind fik, EFilterInstanceProcessResult &fpr);

		PointInstancePtr	getInstance();

		PointInstancePtr getP();
	public:


	private:
		Point				*_parent;
		PointInstancePtr	_instance;
		PointInstancePtr	_superInstance;
		Path				_path;

		TMPoints			_childs;
		TMFilters			_filters;
		TMProperties		_properties;
		TMStatics			_statics;


	private:
		jsval mkFilters(TMFilterInstances &filters, TMFilters &s);
		jsval mkProperties(TMPropertyInstances &properties, TMProperties &s);
		jsval mkChilds(TMPointInstances &childs, TMPoints &s);
		jsval mkStatics(TMStaticInstances &statics, TMStatics &s);

		template <class TMap>
		void mapDeinit(TMap &m);

	public:
		bool call_resolve(uintN argc, jsval *argv, jsval *rval);
		bool call_resolveChild(uintN argc, jsval *argv, jsval *rval);
		bool call_resolveStatic(uintN argc, jsval *argv, jsval *rval);
		bool call_resolveSelf(uintN argc, jsval *argv, jsval *rval);

		bool call_access(uintN argc, jsval *argv, jsval *rval);
		
		bool call_mapConfig(uintN argc, jsval *argv, jsval *rval);
		bool call_map(uintN argc, jsval *argv, jsval *rval);

		bool call_process(uintN argc, jsval *argv, jsval *rval);
		bool call_render(uintN argc, jsval *argv, jsval *rval);

	private:
		bool checkResolveArgs(uintN argc, jsval *argv);
		bool mapConfigAllowFor(jsval mapConfig, const char *val, bool &allow);
	};


	//////////////////////////////////////////////////////////////////////////
	template <class TMap>
	void Point::mapDeinit(TMap &m)
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
