#ifndef _ccms_router_h_
#define _ccms_router_h_

#include "iTransport.hpp"

#include "router/path.hpp"
#include "router/execContext.hpp"
#include "router/contextDataStorage.hpp"

#include "router/cache.hpp"
#include "router/crontab.hpp"
#include "router/plugins.hpp"
#include "router/request.hpp"
#include "i18n/i18n.hpp"

#include "scripter/scripter.hpp"
#include "scripter/jsObject.hpp"
#include "scripter/newPropsCollector.hpp"
#include "scripter/profiler.hpp"

#include "struct/pointInstance.hpp"
#include "struct/point.hpp"


namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class Router
		: public ITransportBackend
		, public JsObject
	{
	public:



		PointInstancePtr		fromConf_pointInstance(const char *file, const char *from);
		bool					fromConf_pointInstances(TMPointInstances &m, const char *file, const char *from);
		PropertyInstancePtr		fromConf_propertyInstance(const char *file, const char *from);
		bool					fromConf_propertyInstances(TMPropertyInstances &m, const char *file, const char *from);
		FilterInstancePtr		fromConf_filterInstance(const char *file, const char *from);
		bool					fromConf_filterInstances(TMFilterInstances &m, const char *file, const char *from);

		PointInstancePtr		fromConf_pointInstance(jsval conf, const char *from);
		bool					fromConf_pointInstanceWithSuper(jsval conf, const char *from, PointInstancePtr res, PointInstancePtr super);
		bool					fromConf_pointInstances(TMPointInstances &m, jsval conf, const char *from);
		PropertyInstancePtr		fromConf_propertyInstance(jsval conf, const char *from);
		bool					fromConf_propertyInstances(TMPropertyInstances &m, jsval conf, const char *from);
		FilterInstancePtr		fromConf_filterInstance(jsval conf, const char *from);
		bool					fromConf_filterInstances(TMFilterInstances &m, jsval conf, const char *from);

		bool					fromConf_names(std::set<std::string> &s, jsval conf, const char *from);
		bool					fromConf_staticInstances(TMStaticInstances &m, jsval conf, const char *from);
		StaticInstancePtr		fromConf_staticInstance(jsval conf, const char *from);

	public:
		jsval	toConf_pointInstance		(PointInstancePtr);
		jsval	toConf_pointInstances		(const TMPointInstances &m);
		jsval	toConf_propertyInstance		(PropertyInstancePtr);
		jsval	toConf_propertyInstances	(const TMPropertyInstances &m);
		jsval	toConf_filterInstance		(FilterInstancePtr);
		jsval	toConf_filterInstances		(const TMFilterInstances &m);
		jsval	toConf_staticInstance		(StaticInstancePtr);
		jsval	toConf_staticInstances		(const TMStaticInstances &m);
		jsval	toConf_names				(const TSNames &s);



	public:

		void					setRootInstance(PointInstancePtr point);
		PointPtr				createPoint(Point *parent, PointInstancePtr pointInstance, PointInstancePtr superPointInstance, const Path &path);

		//что либо внутри структуры изменено, по завершении запроса перебить структуру
		void structChanged(const Path &where);
	public:
		Router();
		~Router();

		bool create(const char *fname);
		jsval getConfigValue(const char *name, JSObject *obj=NULL);
		std::string getConfigString(const char *name);
		long getConfigLong(const char *name);
		unsigned long getConfigUlong(const char *name);
		bool getConfigBool(const char *name);
		double getConfigDouble(const char *name);
		std::map<std::string, std::string> getConfigMapString(const char *name);
		std::map<std::string, int> getConfigMapStringInt(const char *name);
		std::vector<std::string> getConfigArrayString(const char *name);

		//virtual void process(TEnvMap &env, std::istream &in, std::ostream &out, std::ostream &err);

		virtual bool initializeThread();
		virtual void deinitializeThread();
		virtual bool probe(Connection4Backend *connection);
		virtual bool process(Connection4Backend *connection);
		virtual void cleanup(void *connectionData);
		virtual void cronTick(std::ostream &err);

		bool preStart();
		void postStart();


		bool buildExecutePlan(const Path &path, 
			JSObject *executePlan, 
			JSObject *executePlanData, 
			bool &res,
			std::string &staticPath,
			std::vector<PointPtr> &points);
		bool executeForHeaders(Request *r, const std::vector<PointPtr> &points);
		bool executeForBody(Request *r, const std::vector<PointPtr> &points);

#if USE_PROFILER
		std::size_t _profilerAccumuleRequests;
		std::ofstream _profilerLog;
		std::vector<int> _profilerFields;
		int _profilerOrderField;
		size_t _profilerLinesAmount;
		size_t _profilerAccumuledAmount;
		int parseProfilerField(const char *);
		void writeProfilerResult();
#endif

		Scripter &getScripter();
		PointInstancePtr getRootPointInstance();
		PointPtr getRootPoint();

		void evalPossibleSuperPaths(const Path &inh, TVPaths &supers);
		bool isPossibleSuperPath(const Path &inh, const Path &super);


		//далее js контекст
		virtual void jsCreate();
		virtual void jsDestroy();
		virtual void vdeleterJs();

	public:
		//getPoint('/path/to/point')
		bool call_getPointInstance(uintN argc, jsval *argv, jsval *rval);
		//setPoint('/path/to/point', point)
		bool call_setPointInstance(uintN argc, jsval *argv, jsval *rval);

		//getPoint('/path/to/point')
		bool call_getPoint(uintN argc, jsval *argv, jsval *rval);

		//evalPossibleSuperPaths('/path/to/point') return paths array 
		bool call_evalPossibleSuperPaths(uintN argc, jsval *argv, jsval *rval);

		//isPossibleSuperPath('/path/to/inhPoint', '/path/to/superPoint') return paths array 
		bool call_isPossibleSuperPath(uintN argc, jsval *argv, jsval *rval);

		//createPointInstance(conf)
		bool call_createPointInstance(uintN argc, jsval *argv, jsval *rval);

		//createStaticInstance(conf)
		bool call_createFilterInstance(uintN argc, jsval *argv, jsval *rval);

		//createPropertyInstance(owner, conf)
		bool call_createPropertyInstance(uintN argc, jsval *argv, jsval *rval);

		//createStaticInstance(conf)
		bool call_createStaticInstance(uintN argc, jsval *argv, jsval *rval);

		//let conf = router.getPointConf(point);
		bool call_getPointConf(uintN argc, jsval *argv, jsval *rval);
	public:
		//contextData
		//cdStartInstance(key)
		bool call_cdStartInstance(uintN argc, jsval *argv, jsval *rval);
		//cdStopInstance(key)
		bool call_cdStopInstance(uintN argc, jsval *argv, jsval *rval);
		//dataObj = cdGetInstance(key)
		bool call_cdGetInstance(uintN argc, jsval *argv, jsval *rval);
		//cdSetInstance(key, dataObj)
		bool call_cdSetInstance(uintN argc, jsval *argv, jsval *rval);

	public:
		//createService('name')
		bool call_createService(uintN argc, jsval *argv, jsval *rval);

	public:
		//createService('name')
		bool call_createTemplate(uintN argc, jsval *argv, jsval *rval);
		
	public:
		bool call_getConfig(uintN argc, jsval *argv, jsval *rval);

	private:
		void evalPossibleSuperPaths_impl(const Path &inh, TVPaths &supers, const PointInstancePtr &iter);

	private:
		JSObject						*_config;
		boost::filesystem::path			_documentRoot;
		Scripter						_scripter;
		NewPropsCollector				_newPropsCollector;
		PointInstancePtr				_rootInstance;
		PointPtr						_root;
		ContextDataStorage				_contextDataStorage;
		CachePtr						_cache;
		CrontabPtr						_crontab;
		PluginsPtr						_plugins;
		I18nPtr							_i18n;

		size_t	_gcNormalBytes;
		double	_cacheFlushPart;
		time_t	_cacheAliveTime;

		jsuword _stackLimit;

	private:
		//отложенное построение унаследованных поинтов для которых еще не построены суперы
		struct DelayedPointInstanceBuild
		{
			PointInstancePtr	_pointInstance;
			jsval				_conf;
			std::string			_from;

			Path				_superPath;
		};

		typedef std::vector<DelayedPointInstanceBuild> TVDelayedPointInstanceBuilds;
		TVDelayedPointInstanceBuilds	_delayedPointInstanceBuilds;
		PointInstancePtr				_topRoot4DelayedPointInstanceBuilds;

	};

	//////////////////////////////////////////////////////////////////////////
	struct ConnectionData
	{
		RequestPtr _request;
		std::vector<PointPtr> _points;
	};

}
#endif
