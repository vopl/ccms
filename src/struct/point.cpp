#include "stdafx.h"
#include "struct/point.hpp"
#include "struct/property.hpp"
#include "utils/crc32.hpp"
#include "exceptions.hpp"
#include "select2nd.hpp"
#include "router/request.hpp"

namespace ccms
{

	//////////////////////////////////////////////////////////////////////////
	Point::Point(
		Point *parent,
		PointInstancePtr instance, 
		PointInstancePtr superInstance, 
		const Path &path)
		: _parent(parent)
		, _instance(instance)
		, _superInstance(superInstance?superInstance:instance->getSuper())
		, _path(path)

		, JsObject(true, "[object Point]")
	{
		jsRegisterMeth("resolve", &Point::call_resolve, 2);
		jsRegisterMeth("resolveStatic", &Point::call_resolveStatic, 2);
		jsRegisterMeth("resolveChild", &Point::call_resolveChild, 2);
		jsRegisterMeth("resolveSelf", &Point::call_resolveSelf, 2);

		jsRegisterMeth("map", &Point::call_map, 0);
		jsRegisterMeth("mapConfig", &Point::call_mapConfig, 0);

		jsRegisterMeth("render", &Point::call_render, 0);
		jsRegisterMeth("process", &Point::call_process, 0);
	}

	//////////////////////////////////////////////////////////////////////////
	void Point::init()
	{
		if(isInited())
		{
			return;
		}

		JSContext *cx = ecx()->_jsCx;

		{

			std::string path = "#point:/" + _path.string();
			unsigned crc32 = Crc32(path.c_str(), path.size());

			ContextDataPtr cd = mkp<ContextData>(new ContextData((void *)crc32), ROOTNAME);
			jsRegisterProp("cd", cd->thisJsval());
		}

		//////////////////////////////////////////////////////////////////////////
		jsRegisterProp("path", "/" + _path.string());
		jsRegisterProp("id", getId("p"));

		if(_superInstance)
		{
			jsRegisterProp("superInstance", _superInstance->thisJsval());
		}

		//////////////////////////////////////////////////////////////////////////
		TMFilterInstances filters;
		_instance->getFilters_inh(filters);
		jsRegisterProp("filters", mkFilters(filters, _filters));
		filters.clear();

		//////////////////////////////////////////////////////////////////////////
		TMPropertyInstances properties;
		_instance->getProperties_inh(properties);
		jsRegisterProp("properties", mkProperties(properties, _properties));

		//////////////////////////////////////////////////////////////////////////
		TMPointInstances childs;
		_instance->getChilds_inh(childs);
		jsRegisterProp("childs", mkChilds(childs, _childs));

		//////////////////////////////////////////////////////////////////////////
		TMStaticInstances statics;
		_instance->getStatics_inh(statics);
		jsRegisterProp("statics", mkStatics(statics, _statics));

		if(_parent)
		{
			jsRegisterProp("parent", _parent->thisJsval());
		}

		jsRegisterProp("instance", _instance->thisJsval());

		//setPrototype(_instance);
		(JSExceptionReporter)JS_SetPrototype(ecx()->_jsCx, thisJsobj(), _instance->thisJsobj());

		Initializable::init();
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	void Point::deinit()
	{
		if(!isInited())
		{
			return;
		}

		Initializable::deinit();

		//setPrototype(JsPtr());
		(JSExceptionReporter)JS_SetPrototype(ecx()->_jsCx, thisJsobj(), NULL);

		mapDeinit(_childs);
		mapDeinit(_statics);
		mapDeinit(_properties);
		mapDeinit(_filters);

		deletePermanent("path");
		deletePermanent("id");
		deletePermanent("superInstance");

		deletePermanent("filters");

		deletePermanent("controller");
		deletePermanent("properties");
		deletePermanent("childs");
		deletePermanent("statics");
		deletePermanent("parent");

		deletePermanent("instance");

		getPropertyPtr<ContextData>("cd")->unregister();
		deletePermanent("cd");

	}

	//////////////////////////////////////////////////////////////////////////
	void Point::structChanged(const Path &where)
	{
		//самого себя
		if(_instance && where == _instance->getPath())
		{
			reinit();
			return;
		}

		//супера
		if(_superInstance && where == _superInstance->getPath())
		{//супер был изменен
			reinit();
			return;
		}

		//пробить вглубь
		TMPoints::Seq::iterator iter = _childs.seq.begin();
		TMPoints::Seq::iterator end = _childs.seq.end();
		for(; iter!=end; iter++)
		{
			iter->second->structChanged(where);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	Point::~Point()
	{
		deinit();
	}

	//////////////////////////////////////////////////////////////////////////
	const Path &Point::getPath()
	{
		return _path;
	}
	
	//////////////////////////////////////////////////////////////////////////
	const std::string Point::getId(const char *prefix)
	{
		std::string path = "/" + _path.string();
		unsigned crc32 = Crc32(path.c_str(), path.size());

		char tmp[256];
		sprintf(tmp, "%s_%08x", prefix?prefix:"", crc32);

		return tmp;
	}

	//////////////////////////////////////////////////////////////////////////
	PointPtr Point::point4Path(const Path &path)
	{
		if(path.empty())
		{
			return mkp(this, ROOTNAME);
		}

		TMPoints::Assoc::iterator iter = _childs.assoc.find(path.front());
		if(_childs.assoc.end() == iter)
		{
			return PointPtr();
		}

		return iter->second->point4Path(Path(path.begin()+1, path.end()));
	}

	//////////////////////////////////////////////////////////////////////////
	bool Point::executeFilters(EFilterInstanceKind fik, EFilterInstanceProcessResult &fpr)
	{
		fpr = efiprPartial;

		switch(fik)
		{
		case efikPreTransit:
		case efikPreThis:
			BOOST_FOREACH(const TMFilters::Element &el, _filters.seq)
			{
				if(fik == el.second->getKind())
				{
					if(!el.second->process(fpr)) return false;
				}
			}
			break;
		case efikPostThis:
		case efikPostTransit:
			BOOST_REVERSE_FOREACH(const TMFilters::Element &el, _filters.seq)
			{
				if(fik == el.second->getKind())
				{
					if(!el.second->process(fpr)) return false;
				}
			}
			break;
		default:
			assert("wtf?");
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	PointInstancePtr Point::getInstance()
	{
		return _instance;
	}

	//////////////////////////////////////////////////////////////////////////
	PointInstancePtr Point::getP()
	{
		return _instance;
	}


	//////////////////////////////////////////////////////////////////////////
	jsval Point::mkFilters(TMFilterInstances &filters, TMFilters &s)
	{
		JSContext *cx = ecx()->_jsCx;

		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		TMFilterInstances::Seq::iterator iter = filters.seq.begin();
		TMFilterInstances::Seq::iterator end = filters.seq.end();

		for(; iter!=end; iter++)
		{
			FilterPtr filter = mkp(new Filter(this, iter->second), ROOTNAME);
			filter->init();
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				iter->first.c_str(), 
				filter->thisJsval(),
				NULL, NULL, 
				JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT);
			s.seq.push_back(s.mk(iter->first, filter));
		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	jsval Point::mkProperties(TMPropertyInstances &properties, TMProperties &s)
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
	jsval Point::mkChilds(TMPointInstances &childs, TMPoints &s)
	{
		JSContext *cx = ecx()->_jsCx;

		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		TMPointInstances superChilds;//это дочерние от супера, пришедшего по иерархии родительский/дочерний
		if(_superInstance && _superInstance != _instance->getSuper())
		{
			_superInstance->getChilds_inh(superChilds);
		}

		TMPointInstances::Seq::iterator iter = childs.seq.begin();
		TMPointInstances::Seq::iterator end = childs.seq.end();
		for(; iter!=end; iter++)
		{
			PointInstancePtr pointInstance = iter->second;

			TMPointInstances::Assoc::iterator superPointInstance_iter = superChilds.assoc.find(iter->first);
			PointInstancePtr superPointInstance;
			if(superChilds.assoc.end() != superPointInstance_iter)
			{
				superPointInstance = superPointInstance_iter->second;
			}

			PointPtr point = mkp(new Point(this, pointInstance, superPointInstance, _path/iter->first.c_str()), ROOTNAME);
			point->init();
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				iter->first.c_str(), 
				point->thisJsval(),
				NULL, NULL, 
				JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT);
			s.seq.push_back(s.mk(iter->first, point));
		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	jsval Point::mkStatics(TMStaticInstances &statics, TMStatics &s)
	{
		JSContext *cx = ecx()->_jsCx;

		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		TMStaticInstances superStatics;//это статики от супера, пришедшего по иерархии родительский/дочерний
		if(_superInstance && _superInstance != _instance->getSuper())
		{
			_superInstance->getStatics_inh(superStatics);
		}

		TMStaticInstances::Seq::iterator iter = statics.seq.begin();
		TMStaticInstances::Seq::iterator end = statics.seq.end();
		for(; iter!=end; iter++)
		{
			StaticPtr static_ = mkp(new Static(this, iter->second), ROOTNAME);
			static_->init();
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				iter->first.c_str(), 
				static_->thisJsval(),
				NULL, NULL, 
				JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT);
			s.seq.push_back(s.mk(iter->first, static_));
		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Point::call_resolve(uintN argc, jsval *argv, jsval *rval)
	{
		//если есть свойство то дернуть его
		{
			TMProperties::Assoc::iterator iter = _properties.assoc.find("resolve");
			if(_properties.assoc.end() != iter)
			{
				return iter->second->call_render(argc, argv, rval);
			}
		}

		//по умолчанию резолвить по структуре точек
		if(!call_resolveStatic(argc, argv, rval)) return false;
		if(JSVAL_IS_OBJECT(*rval)) return true;

		if(!call_resolveChild(argc, argv, rval)) return false;
		if(JSVAL_IS_OBJECT(*rval)) return true;

		*rval = JSVAL_FALSE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Point::call_resolveChild(uintN argc, jsval *argv, jsval *rval)
	{
		//если есть свойство то дернуть его
		{
			TMProperties::Assoc::iterator iter = _properties.assoc.find("resolveChild");
			if(_properties.assoc.end() != iter)
			{
				return iter->second->call_render(argc, argv, rval);
			}
		}

		if(!checkResolveArgs(argc, argv)) return false;

		char *pathFront;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &pathFront)) return false;

		//искать ключ среди дочерних
		jsval jsv;
		if(!call_mapConfig(0, NULL, &jsv)) return false;
		bool allowChilds;
		if(!mapConfigAllowFor(jsv, "childs", allowChilds)) return false;

		if(allowChilds)
		{
			TMPoints::Assoc::iterator iter = _childs.assoc.find(pathFront);

			if(_childs.assoc.end() != iter)
			{
				//если найден дочерний
				if(!iter->second->call_mapConfig(0, NULL, &jsv)) return false;
				bool allowSelf;
				if(!mapConfigAllowFor(jsv, "self", allowSelf)) return false;

				if(allowSelf)
				{
					//положить как точку
					JSObject *res = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
					if(!res) return false;
					if(!JS_DefineProperty(
						ecx()->_jsCx, 
						res, 
						"point", 
						iter->second->thisJsval(),
						NULL, NULL, JSPROP_ENUMERATE)) return false;

					*rval = OBJECT_TO_JSVAL(res);
					return true;
				}
			}
		}

		//ни дочерний ни статик не найден, это неудача
		*rval = JSVAL_FALSE;
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Point::call_resolveStatic(uintN argc, jsval *argv, jsval *rval)
	{
		//если есть свойство то дернуть его
		{
			TMProperties::Assoc::iterator iter = _properties.assoc.find("resolveStatic");
			if(_properties.assoc.end() != iter)
			{
				return iter->second->call_render(argc, argv, rval);
			}
		}

		if(!checkResolveArgs(argc, argv)) return false;

		char *pathFront;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &pathFront)) return false;

		//искать ключ среди дочерних
		{
			TMStatics::Assoc::iterator iter = _statics.assoc.find(pathFront);

			if(_statics.assoc.end() != iter)
			{
				//если найден дочерний
				//положить как точку
				JSObject *res = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
				if(!res) return false;
				if(!JS_DefineProperty(
					ecx()->_jsCx, 
					res, 
					"static", 
					iter->second->thisJsval(),
					NULL, NULL, JSPROP_ENUMERATE)) return false;

				*rval = OBJECT_TO_JSVAL(res);
				return true;
			}
		}

		//ни дочерний ни статик не найден, это неудача
		*rval = JSVAL_FALSE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Point::call_resolveSelf(uintN argc, jsval *argv, jsval *rval)
	{
		//если есть свойство то дернуть его
		{
			TMProperties::Assoc::iterator iter = _properties.assoc.find("resolveSelf");
			if(_properties.assoc.end() != iter)
			{
				return iter->second->call_render(argc, argv, rval);
			}
		}

		if(argc != 2)
		{
			JS_ReportError(ecx()->_jsCx, "Point.resolveSelf must be called with 2 args");
			return false;
		}

		*rval = argv[0];
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Point::call_mapConfig(uintN argc, jsval *argv, jsval *rval)
	{
		//если есть свойство то дернуть его
		{
			TMProperties::Assoc::iterator iter = _properties.assoc.find("mapConfig");
			if(_properties.assoc.end() != iter)
			{
				return iter->second->call_render(argc, argv, rval);
			}
		}

		*rval = JSVAL_TRUE;
		return true;

		JSObject *res = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		if(!res) return false;

		if(!JS_DefineProperty(
			ecx()->_jsCx, 
			res, 
			"self", 
			JSVAL_TRUE,
			NULL, NULL, JSPROP_ENUMERATE)) return false;
		if(!JS_DefineProperty(
			ecx()->_jsCx, 
			res, 
			"childs", 
			JSVAL_TRUE,
			NULL, NULL, JSPROP_ENUMERATE)) return false;

		*rval = OBJECT_TO_JSVAL(res);
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Point::call_map(uintN argc, jsval *argv, jsval *rval)
	{
		//если есть свойство то дернуть его
		{
			TMProperties::Assoc::iterator iter = _properties.assoc.find("map");
			if(_properties.assoc.end() != iter)
			{
				return iter->second->call_render(argc, argv, rval);
			}
		}


		//дочерние
		/*
		{
			title:'tratata',//*
			point:Point,
			childs:{},
		}
		*/

		//////////////////////
		JSObject *res = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		if(!res) return false;

		if(!JS_DefineProperty(
			ecx()->_jsCx, 
			res, 
			"point", 
			thisJsval(),
			NULL, NULL, JSPROP_ENUMERATE)) return false;

		bool allowChilds = true;
		if(argc)
		{
			if(!mapConfigAllowFor(argv[0], "childs", allowChilds)) return false;
		}


		if(allowChilds)
		{
			jsval jsv;
			if(!call_mapConfig(0, NULL, &jsv)) return false;

			bool allowSelf;
			if(!mapConfigAllowFor(jsv, "self", allowSelf)) return false;
			if(!allowSelf)
			{
				*rval = JSVAL_VOID;
				return true;
			}

			bool allowChilds;
			if(!mapConfigAllowFor(jsv, "childs", allowChilds)) return false;


			//////////////////////
			if(allowChilds)
			{
				JSObject *childs = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
				if(!childs) return false;

				{
					TMPoints::Seq::iterator iter = _childs.seq.begin();
					TMPoints::Seq::iterator end = _childs.seq.end();

					for(; iter!=end; iter++)
					{
						if(allowSelf)
						{
							if(!iter->second->call_map(argc, argv, &jsv)) return false;

							if(JSVAL_VOID != jsv)
							{
								if(!JS_DefineProperty(
									ecx()->_jsCx, 
									childs, 
									iter->first.data(), 
									jsv,
									NULL, NULL, JSPROP_ENUMERATE)) return false;
							}
						}
					}
				}

				if(!JS_DefineProperty(
					ecx()->_jsCx, 
					res, 
					"childs", 
					OBJECT_TO_JSVAL(childs),
					NULL, NULL, JSPROP_ENUMERATE)) return false;
			}
		}

		*rval = OBJECT_TO_JSVAL(res);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Point::call_process(uintN argc, jsval *argv, jsval *rval)
	{
		TMProperties::Assoc::iterator iter = _properties.assoc.find("process");
		if(_properties.assoc.end() != iter)
		{
			return iter->second->call_render(argc, argv, rval);
		}
		//throw exception("'process' is not setted in point");
		*rval = JSVAL_VOID;
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Point::call_render(uintN argc, jsval *argv, jsval *rval)
	{
		TMProperties::Assoc::iterator iter = _properties.assoc.find("render");
		if(_properties.assoc.end() != iter)
		{
			return iter->second->call_render(argc, argv, rval);
		}
		//throw exception("'render' is not setted in point");
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Point::checkResolveArgs(uintN argc, jsval *argv)
	{
		if(argc != 2)
		{
			JS_ReportError(ecx()->_jsCx, "Point.resolve must be called with 2 args");
			return false;
		}

		if(!JSVAL_IS_STRING(argv[0]))
		{
			JS_ReportError(ecx()->_jsCx, "Point.resolve 1st arg must be a String");
			return false;
		}

		if(!JSVAL_IS_OBJECT(argv[1]) || JSVAL_IS_NULL(argv[1]))
		{
			JS_ReportError(ecx()->_jsCx, "Point.resolve 2nd arg must be a Object");
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Point::mapConfigAllowFor(jsval mapConfig, const char *val, bool &allow)
	{
		allow = false;

		if(JSVAL_TRUE == mapConfig)
		{
			allow = true;
			return true;
		}

		if(JSVAL_IS_NULL(mapConfig) || JSVAL_IS_VOID(mapConfig) || JSVAL_FALSE == mapConfig)
		{
			allow = false;
			return true;
		}

		if(JSVAL_IS_STRING(mapConfig))
		{
			const char *s;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &mapConfig, "s", &s)) return false;

			allow = 0 == strcmp(s, val);
			return true;
		}

		if(JSVAL_IS_OBJECT(mapConfig))
		{
			JSObject *jso = JSVAL_TO_OBJECT(mapConfig);
			jsval jsv;
			if(!JS_GetProperty(ecx()->_jsCx, jso, val, &jsv)) return false;

			JSBool b;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "b", &b)) return false;

			allow = b?true:false;
			return true;
		}

		allow = false;
		return true;
	}

}
