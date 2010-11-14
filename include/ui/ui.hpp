#ifndef _ccms_ui_hpp_
#define _ccms_ui_hpp_

#include "scripter/jsObject.hpp"
#include "router/path.hpp"
#include "router/contextData.hpp"
#include "struct/point.hpp"
#include "ui/hookPoint.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class Ui
		: public JsObject
	{
		//слоты
		enum ESlot
		{
			es_skinProvider=0,
			es_layoutProvider=1,
			es_blockProvider=2,
			es_skinName=3,
			es_layoutName=4,
			es_blocks=5,

			//es_skinPoint=6,
			//es_layoutPoint=7,
			//es_blockPoints=8,
		};
	public:
		Ui();
		~Ui();

	private:
		Path _pathRoot;
		PointPtr _currentLayoutPoint;
		PointPtr _currentSkinPoint;

	private:

		bool xetter_skinProvider(jsval *vp, bool isGet);
		bool xetter_layoutProvider(jsval *vp, bool isGet);
		bool xetter_blockProvider(jsval *vp, bool isGet);

	private:
		bool call_reset(uintN argc, jsval *argv, jsval *rval);

		bool xetter_layoutName(jsval *vp, bool isGet);
		bool xetter_skinName(jsval *vp, bool isGet);
		bool xetter_blocks(jsval *vp, bool isGet);

		bool xetter_layouts(jsval *vp, bool isGet);
		bool xetter_skins(jsval *vp, bool isGet);
		bool xetter_places(jsval *vp, bool isGet);

		bool xetter_skin(jsval *vp, bool isGet);

		bool call_print(uintN argc, jsval *argv, jsval *rval);

		bool call_printHelper(jsval val, jsval *rval);

	public:
		bool call_hookPlace(uintN argc, jsval *argv, jsval *rval);
		bool call_hookInvoke(uintN argc, jsval *argv, jsval *rval);
		bool call_hookSubscribe(uintN argc, jsval *argv, jsval *rval);
		bool call_hookUnsubscribe(uintN argc, jsval *argv, jsval *rval);

	private:
		Path &getPathRoot();

		bool currentLayout(Point *&p);
		bool currentSkin(Point *&p);
		bool currentBlocks(const char *placeName, jsval &rval);

		JSObject *mkEmptyBlocks();
		JSObject *mkNamedFlags(JSObject *names);

	private:
		class Hook
		{
			struct TSubscriber
				: public std::pair<JSObject *, jsval>
			{
				TSubscriber(JSObject *o, const jsval &v)
					: std::pair<JSObject *, jsval>(o, v)
				{}

				bool operator<(const TSubscriber &with) const
				{
					if(first < with.first) return true;
					if(first > with.first) return false;
					if(second < with.second) return true;
					return false;
				}
			};
			typedef std::map<TSubscriber, int> TMSubscribers;
			TMSubscribers _subscribers;

		public:
			bool invoke(THookId id, jsval data, jsval *rval);
			int subscribe(JSObject *obj, jsval meth, bool enable);
		};
		typedef std::map<THookId, Hook> TMHooks;
		TMHooks _hooks;

	private:
		THookId hookCreate(jsval id);
		THookId hookGetId(jsval id, jsval *data=NULL, bool parseObj = true);
		bool hookInvoke(THookId id, jsval data, jsval *rval);
		bool hookSubscribe(THookId id, JSObject *obj, jsval meth, bool enable);
	};
}

#endif
