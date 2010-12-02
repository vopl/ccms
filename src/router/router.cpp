#include "stdafx.h"
#include "router/router.hpp"
#include "router/request.hpp"
#include "exceptions.hpp"
#include "db/sqlite3.hpp"
#include "db/pg.hpp"
#include "tpm/template.hpp"
#include "ui/ui.hpp"
#include "utils/crc32.hpp"
#include "utils/string.hpp"

#include "magick/magick.hpp"
#include "mime/mime.hpp"
#include "net/net.hpp"
#include "hash/hash.hpp"

#include "router/log.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	extern ExecContext *ecx_rw();


	//////////////////////////////////////////////////////////////////////////
	PointInstancePtr Router::fromConf_pointInstance(const char *file, const char *from)
	{
		std::string fname = _scripter.resolveLibName(file, from);
		if(fname.empty())
		{
			_topRoot4DelayedPointInstanceBuilds.reset();
			throw exception_noFile(file, from);
			return PointInstancePtr();
		}

		jsval v = _scripter.f_execFile(fname);
		return fromConf_pointInstance(v, fname.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::fromConf_pointInstances(TMPointInstances &m, const char *file, const char *from)
	{
		std::string fname = _scripter.resolveLibName(file, from);
		if(fname.empty())
		{
			throw exception_noFile(file, from);
			return PointInstancePtr();
		}

		jsval v = _scripter.f_execFile(fname);
		return fromConf_pointInstances(m, v, fname.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	PropertyInstancePtr Router::fromConf_propertyInstance(const char *file, const char *from)
	{
		std::string fname = _scripter.resolveLibName(file, from);
		if(fname.empty())
		{
			throw exception_noFile(file, from);
			return PropertyInstancePtr();
		}

		jsval v = _scripter.f_execFile(fname);
		return fromConf_propertyInstance(v, fname.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::fromConf_propertyInstances(TMPropertyInstances &m, const char *file, const char *from)
	{
		std::string fname = _scripter.resolveLibName(file, from);
		if(fname.empty())
		{
			throw exception_noFile(file, from);
			return PropertyInstancePtr();
		}

		jsval v = _scripter.f_execFile(fname);
		return fromConf_propertyInstances(m, v, fname.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	FilterInstancePtr Router::fromConf_filterInstance(const char *file, const char *from)
	{
		std::string fname = _scripter.resolveLibName(file, from);
		if(fname.empty())
		{
			throw exception_noFile(file, from);
			return FilterInstancePtr();
		}

		jsval v = _scripter.f_execFile(fname);
		return fromConf_filterInstance(v, fname.c_str());
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::fromConf_filterInstances(TMFilterInstances &m, const char *file, const char *from)
	{
		std::string fname = _scripter.resolveLibName(file, from);
		if(fname.empty())
		{
			throw exception_noFile(file, from);
			return PropertyInstancePtr();
		}

		jsval v = _scripter.f_execFile(fname);
		return fromConf_filterInstances(m, v, fname.c_str());
	}


	//////////////////////////////////////////////////////////////////////////
	PointInstancePtr Router::fromConf_pointInstance(jsval conf, const char *from)
	{
		if(!from)
		{
			from = "";
		}

		if(JSVAL_IS_STRING(conf))
		{//путь к конфу
			char *s;
			(JSExceptionReporter)JS_ConvertArguments(ecx()->_jsCx, 1, &conf, "s", &s);
			return fromConf_pointInstance(s, from);
		}


		if(!JSVAL_IS_OBJECT(conf) || JSVAL_IS_NULL(conf))
		{
			_topRoot4DelayedPointInstanceBuilds.reset();
			throw(exception(std::string(from) + ": conf for point must be an object"));
			return PointInstancePtr();
		}

		PointInstancePtr res = JsObject::mkp<PointInstance>(new PointInstance, ROOTNAME);

		bool localRoot = false;
		if(!_topRoot4DelayedPointInstanceBuilds)
		{
			localRoot = true;
			_topRoot4DelayedPointInstanceBuilds = res;
		}

		JSContext *cx = ecx()->_jsCx;
		JSObject *obj = JSVAL_TO_OBJECT(conf);
		jsval rval;
		char *s;
		JSBool b;


		//////////////////////////////////////////////////////////////////////////
		PointInstancePtr super;
		if(	JS_HasProperty(cx, obj, "super", &b) && b &&
			JS_GetProperty(cx, obj, "super", &rval))
		{
			if(!JSVAL_IS_STRING(rval))
			{
				_topRoot4DelayedPointInstanceBuilds.reset();
				throw exception(std::string(from) + ": super in conf for point must be an string-path for another point");
				return PointInstancePtr();
			}
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &rval, "s", &s);




			Path superPath = s;
			super = _topRoot4DelayedPointInstanceBuilds->point4Path(superPath);

			if(!super && _rootInstance)
			{
				super = _rootInstance->point4Path(superPath);
			}

// 			if(!super && _root)
// 			{
// 				PointPtr super = _root->point4Path(superPath);
// 				if(super)
// 				{
// 					super = super->getP();
// 				}
// 			}

			if(!super)
			{
				if(localRoot)
				{//на самом верхнем уровне и заданный супер не найден
					_topRoot4DelayedPointInstanceBuilds.reset();
					throw exception(std::string(from) + ": in conf for point unable to determine super ("+s+")");
					return PointInstancePtr();
				}

				//построить позже
				_delayedPointInstanceBuilds.push_back(DelayedPointInstanceBuild());
				DelayedPointInstanceBuild &dpib = _delayedPointInstanceBuilds.back();
				dpib._conf = conf;
				dpib._from = from;
				dpib._pointInstance = res;
				dpib._superPath = s;
				return res;
			}

		}

		//достроить этот поинт
		fromConf_pointInstanceWithSuper(conf, from, res, super);

		if(localRoot)
		{
			_topRoot4DelayedPointInstanceBuilds.reset();
			if(_delayedPointInstanceBuilds.size())
			{//на самом верхнем уровне и какойто из суперов глубже не найден
				DelayedPointInstanceBuild dpib = _delayedPointInstanceBuilds.front();
				//_topRoot4DelayedPointInstanceBuilds.reset();
				_delayedPointInstanceBuilds.clear();
				throw exception(std::string(from) + ": in conf for pointInstance unable to determine super (/"+dpib._superPath.string()+")");
				return PointInstancePtr();
			}
		}
		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::fromConf_pointInstanceWithSuper(jsval conf, const char *from, PointInstancePtr res, PointInstancePtr super)
	{
		res->_super = super;

		JSContext *cx = ecx()->_jsCx;
		JSObject *obj = JSVAL_TO_OBJECT(conf);
		jsval rval;
		JSBool b;

		//////////////////////////////////////////////////////////////////////////

#define PR_MAP(type, vname, fname)							\
res->_ ## vname.clear(); res->_ ## vname ## _hidden.clear();		\
if(	JS_HasProperty(cx, obj, #vname, &b) && b &&				\
		JS_GetProperty(cx, obj, #vname, &rval))				\
{															\
	if(!fromConf_ ## fname(res->_ ## vname, rval, from))	\
	{														\
		_topRoot4DelayedPointInstanceBuilds.reset();		\
		throw exception(std::string(from) + ": in conf for point unable to process " #vname);		\
		return PointInstancePtr();									\
	}														\
}															\
if(	JS_HasProperty(cx, obj, #vname "_hidden", &b) && b &&	\
	JS_GetProperty(cx, obj, #vname "_hidden", &rval))		\
{															\
	if(!fromConf_names(res->_ ## vname ## _hidden, rval, from))		\
	{														\
		_topRoot4DelayedPointInstanceBuilds.reset();		\
		throw exception(std::string(from) + ": in conf for point unable to process " #vname "_hidden");	\
		return PointInstancePtr();									\
	}														\
}															//\

		//////////////////////////////////////////////////////////////////////////
		//фильтры
		PR_MAP(TMFilterInstances, filters, filterInstances);

		//////////////////////////////////////////////////////////////////////////
		//пропертя
		if(	JS_HasProperty(cx, obj, "properties", &b) && b &&
			JS_GetProperty(cx, obj, "properties", &rval))
		{
			if(!fromConf_propertyInstances(res->_properties, rval, from))
			{
				_topRoot4DelayedPointInstanceBuilds.reset();
				throw exception(std::string(from) + ": in conf for point unable to process properties");
				return PointInstancePtr();
			}
		}
		if(	JS_HasProperty(cx, obj, "properties_hidden", &b) && b &&
			JS_GetProperty(cx, obj, "properties_hidden", &rval))
		{
			if(!fromConf_names(res->_properties_hidden, rval, from))
			{
				_topRoot4DelayedPointInstanceBuilds.reset();
				throw exception(std::string(from) + ": in conf for point unable to process properties_hidden");
				return PointInstancePtr();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//дочерние
		if(	JS_HasProperty(cx, obj, "childs", &b) && b &&
			JS_GetProperty(cx, obj, "childs", &rval))
		{
			if(!fromConf_pointInstances(res->_childs, rval, from))
			{
				_topRoot4DelayedPointInstanceBuilds.reset();
				throw exception(std::string(from) + ": in conf for point unable to process childs");
				return PointInstancePtr();
			}
		}
		if(	JS_HasProperty(cx, obj, "childs_hidden", &b) && b &&
			JS_GetProperty(cx, obj, "childs_hidden", &rval))
		{
			if(!fromConf_names(res->_childs_hidden, rval, from))
			{
				_topRoot4DelayedPointInstanceBuilds.reset();
				throw exception(std::string(from) + ": in conf for point unable to process childs_hidden");
				return PointInstancePtr();
			}
		}

		{
			TMPointInstances::iterator iter = res->_childs.begin();
			TMPointInstances::iterator end = res->_childs.end();
			for(; iter!=end; iter++)
			{
				iter->second->_parent = res.get();
			}
		}


		//////////////////////////////////////////////////////////////////////////
		//статики
		if(	JS_HasProperty(cx, obj, "statics", &b) && b &&
			JS_GetProperty(cx, obj, "statics", &rval))
		{
			if(!fromConf_staticInstances(res->_statics, rval, from))
			{
				_topRoot4DelayedPointInstanceBuilds.reset();
				throw exception(std::string(from) + ": in conf for point unable to process statics");
				return PointInstancePtr();
			}
		}
		if(	JS_HasProperty(cx, obj, "statics_hidden", &b) && b &&
			JS_GetProperty(cx, obj, "statics_hidden", &rval))
		{
			if(!fromConf_names(res->_statics_hidden, rval, from))
			{
				_topRoot4DelayedPointInstanceBuilds.reset();
				throw exception(std::string(from) + ": in conf for point unable to process childs_hidden");
				return PointInstancePtr();
			}
		}


		//////////////////////////////////////////////////////////////////////////
		//достроить недостроенные унаследованные
		for(size_t i(0); i<_delayedPointInstanceBuilds.size(); )
		{
			//вновь поискать супера, может уже сделался
			Path &superPath = _delayedPointInstanceBuilds[i]._superPath;

			PointInstancePtr super = _topRoot4DelayedPointInstanceBuilds->point4Path(superPath);
			if(!super && _rootInstance)
			{
				super = _rootInstance->point4Path(superPath);
			}

// 			if(!super && _root)
// 			{
// 				PointPtr super = _root->point4Path(superPath);
// 				if(super)
// 				{
// 					super = super->getP();
// 				}
// 			}

			if(super)
			{
				//новые добавлятся будут в конец, так что ничего страшного от удаления из середины
				DelayedPointInstanceBuild dpib = _delayedPointInstanceBuilds[i];
				_delayedPointInstanceBuilds.erase(_delayedPointInstanceBuilds.begin() + i);

				fromConf_pointInstanceWithSuper(dpib._conf, dpib._from.c_str(), dpib._pointInstance, super);
				continue;
			}

			i++;
		}


		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::fromConf_pointInstances(TMPointInstances &m, jsval conf, const char *from)
	{
		m.clear();

		if(JSVAL_IS_NULL(conf) || JSVAL_IS_VOID(conf))
		{
			return true;
		}

		if(!JSVAL_IS_OBJECT(conf))
		{
			throw(exception(std::string(from) + ": conf for points must be an object"));
			return false;
		}

		JSContext *cx = ecx()->_jsCx;
		JSObject *obj = JSVAL_TO_OBJECT(conf);
		jsval rval;
		char *s;

		JSIdArray *ids = JS_Enumerate(cx, obj);

		if(ids)
		{
			for(jsint i(0); i<ids->length; i++)
			{
				jsval idVal;

				if(	JS_IdToValue(cx, ids->vector[i], &idVal) &&
					JS_ConvertArguments(cx, 1, &idVal, "s", &s) &&
					JS_GetPropertyById(cx, obj, ids->vector[i], &rval))
				{
					if(!Path::isValidPathPart(s))
					{
						JS_DestroyIdArray(cx, ids);
						throw exception_badPathPart(s);
						return false;
					}

					PointInstancePtr point = fromConf_pointInstance(rval, from);
					if(!point)
					{
						JS_DestroyIdArray(cx, ids);
						throw exception(std::string(from) + ": unable to create point "+s);
						return PointInstancePtr();
					}
					m.seq.push_back(m.mk(s, point));
				}
			}
			JS_DestroyIdArray(cx, ids);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	PropertyInstancePtr Router::fromConf_propertyInstance(jsval conf, const char *from)
	{
		if(JSVAL_IS_STRING(conf))
		{//путь к конфу
			char *s;
			(JSExceptionReporter)JS_ConvertArguments(ecx()->_jsCx, 1, &conf, "s", &s);
			return fromConf_propertyInstance(s, from);
		}

		if(!JSVAL_IS_OBJECT(conf) || JSVAL_IS_NULL(conf))
		{
			throw(exception(std::string(from) + ": conf for property must be an object"));
			return PropertyInstancePtr();
		}

		JSContext *cx = ecx()->_jsCx;
		JSObject *obj = JSVAL_TO_OBJECT(conf);
		jsval rval;
		char *s;
		JSBool b;

		//////////////////////////////////////////////////////////////////////////
		std::string kind;
		if(	JS_HasProperty(cx, obj, "kind", &b) && b &&
			JS_GetProperty(cx, obj, "kind", &rval))
		{
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &rval, "s", &s);
			kind = s;
		}

		//////////////////////////////////////////////////////////////////////////
		std::string description;
		if(	JS_HasProperty(cx, obj, "description", &b) && b &&
			JS_GetProperty(cx, obj, "description", &rval))
		{
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &rval, "s", &s);
			description = s;
		}

		//////////////////////////////////////////////////////////////////////////
		std::string value;
		if(	JS_HasProperty(cx, obj, "value", &b) && b &&
			JS_GetProperty(cx, obj, "value", &rval))
		{
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &rval, "s", &s);
			value = s;
		}

		//////////////////////////////////////////////////////////////////////////
		std::string file;
		if(	JS_HasProperty(cx, obj, "file", &b) && b &&
			JS_GetProperty(cx, obj, "file", &rval))
		{
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &rval, "s", &s);
			if(s[0])
			{
				file = ecx()->_scripter->resolveLibName(s, from);
				if(file.empty())
				{
					throw exception_noFile(s, from);
					return PropertyInstancePtr();
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		bool traceFile = true;
		if(	JS_HasProperty(cx, obj, "traceFile", &b) && b &&
			JS_GetProperty(cx, obj, "traceFile", &rval))
		{
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &rval, "b", &b);
			traceFile = b?true:false;
		}

		//////////////////////////////////////////////////////////////////////////
		bool executable = true;
		if(	JS_HasProperty(cx, obj, "executable", &b) && b &&
			JS_GetProperty(cx, obj, "executable", &rval))
		{
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &rval, "b", &b);
			executable = b?true:false;
		}

		PropertyInstancePtr res = JsObject::mkp<PropertyInstance>(new PropertyInstance(), ROOTNAME);

		res->_kind = kind;
		res->_value = value;
		res->_description = description;
		res->_executable = executable;

		res->_file = file;
		res->_traceFile = traceFile;


		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::fromConf_propertyInstances(TMPropertyInstances &m, jsval conf, const char *from)
	{
		m.clear();

		if(JSVAL_IS_NULL(conf) || JSVAL_IS_VOID(conf))
		{
			_topRoot4DelayedPointInstanceBuilds.reset();
			return true;
		}

		if(!JSVAL_IS_OBJECT(conf))
		{
			_topRoot4DelayedPointInstanceBuilds.reset();
			throw(exception(std::string(from) + ": conf for properties must be an object"));
			return false;
		}

		JSContext *cx = ecx()->_jsCx;
		JSObject *obj = JSVAL_TO_OBJECT(conf);
		jsval rval;
		char *s;

		JSIdArray *ids = JS_Enumerate(cx, obj);

		if(ids)
		{
			for(jsint i(0); i<ids->length; i++)
			{
				jsval idVal;

				if(	JS_IdToValue(cx, ids->vector[i], &idVal) &&
					JS_ConvertArguments(cx, 1, &idVal, "s", &s) &&
					JS_GetPropertyById(cx, obj, ids->vector[i], &rval))
				{
					if(!Path::isValidPathPart(s))
					{
						JS_DestroyIdArray(cx, ids);
						_topRoot4DelayedPointInstanceBuilds.reset();
						throw exception_badPathPart(s);
						return false;
					}

					PropertyInstancePtr property = fromConf_propertyInstance(rval, from);
					if(!property)
					{
						JS_DestroyIdArray(cx, ids);
						_topRoot4DelayedPointInstanceBuilds.reset();
						throw exception(std::string(from) + ": unable to create property "+s);
						return PropertyInstancePtr();
					}
					m.seq.push_back(m.mk(s, property));
				}
			}
			JS_DestroyIdArray(cx, ids);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	FilterInstancePtr Router::fromConf_filterInstance(jsval conf, const char *from)
	{
		if(JSVAL_IS_STRING(conf))
		{//путь к конфу
			char *s;
			(JSExceptionReporter)JS_ConvertArguments(ecx()->_jsCx, 1, &conf, "s", &s);
			return fromConf_filterInstance(s, from);
		}

		if(!JSVAL_IS_OBJECT(conf) || JSVAL_IS_NULL(conf))
		{
			throw(exception(std::string(from) + ": conf for filter must be an object"));
			return FilterInstancePtr();
		}

		JSContext *cx = ecx()->_jsCx;
		JSObject *obj = JSVAL_TO_OBJECT(conf);
		jsval rval;
		char *s;
		JSBool b;

		//////////////////////////////////////////////////////////////////////////
		FilterInstancePtr res = mkp(new FilterInstance, ROOTNAME);

		std::string kind;
		if(	JS_HasProperty(cx, obj, "kind", &b) && b &&
			JS_GetProperty(cx, obj, "kind", &rval))
		{
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &rval, "s", &s);
			kind = s;
		}
				if(kind.empty())			res->_kind = efikPreTransit;
		else	if(kind == "preTransit")	res->_kind = efikPreTransit;
		else	if(kind == "preThis")		res->_kind = efikPreThis;
		else	if(kind == "postThis")		res->_kind = efikPostThis;
		else	if(kind == "postTransit")	res->_kind = efikPostTransit;
		else
		{
			//res->_kind = efikPreTransit;
			_topRoot4DelayedPointInstanceBuilds.reset();
			throw exception(std::string(from) + ": in conf for filter unknown kind '"+kind+"'");
			return FilterInstancePtr();
		}

		//////////////////////////////////////////////////////////////////////////
		//пропертя
		if(	JS_HasProperty(cx, obj, "properties", &b) && b &&
			JS_GetProperty(cx, obj, "properties", &rval))
		{
			if(!fromConf_propertyInstances(res->_properties, rval, from))
			{
				_topRoot4DelayedPointInstanceBuilds.reset();
				throw exception(std::string(from) + ": in conf for filter unable to process properties");
				return FilterInstancePtr();
			}
		}
		if(	JS_HasProperty(cx, obj, "properties_hidden", &b) && b &&
			JS_GetProperty(cx, obj, "properties_hidden", &rval))
		{
			if(!fromConf_names(res->_properties_hidden, rval, from))
			{
				_topRoot4DelayedPointInstanceBuilds.reset();
				throw exception(std::string(from) + ": in conf for filter unable to process properties_hidden");
				return FilterInstancePtr();
			}
		}


		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::fromConf_filterInstances(TMFilterInstances &m, jsval conf, const char *from)
	{
		m.clear();
		
		if(JSVAL_IS_NULL(conf) || JSVAL_IS_VOID(conf))
		{
			return true;
		}

		if(!JSVAL_IS_OBJECT(conf))
		{
			throw(exception(std::string(from) + ": conf for filters must be an object"));
			return false;
		}

		JSContext *cx = ecx()->_jsCx;
		JSObject *obj = JSVAL_TO_OBJECT(conf);
		jsval rval;
		char *s;

		JSIdArray *ids = JS_Enumerate(cx, obj);

		if(ids)
		{
			for(jsint i(0); i<ids->length; i++)
			{
				jsval idVal;

				if(	JS_IdToValue(cx, ids->vector[i], &idVal) &&
					JS_ConvertArguments(cx, 1, &idVal, "s", &s) &&
					JS_GetPropertyById(cx, obj, ids->vector[i], &rval))
				{
					if(!Path::isValidPathPart(s))
					{
						JS_DestroyIdArray(cx, ids);
						throw exception_badPathPart(s);
						return false;
					}

					FilterInstancePtr filter = fromConf_filterInstance(rval, from);
					if(!filter)
					{
						JS_DestroyIdArray(cx, ids);
						throw exception(std::string(from) + ": unable to create filter "+s);
						return FilterInstancePtr();
					}
					m.seq.push_back(m.mk(s, filter));
				}
			}
			JS_DestroyIdArray(cx, ids);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::fromConf_names(std::set<std::string> &s_, jsval conf, const char *from)
	{
		s_.clear();

		if(JSVAL_IS_NULL(conf) || JSVAL_IS_VOID(conf))
		{
			return true;
		}

		if(!JSVAL_IS_OBJECT(conf))
		{
			throw(exception(std::string(from) + ": conf for names must be an object"));
			return false;
		}

		JSContext *cx = ecx()->_jsCx;
		JSObject *obj = JSVAL_TO_OBJECT(conf);
		jsval rval;
		char *s;
		JSBool b;

		JSIdArray *ids = JS_Enumerate(cx, obj);

		if(ids)
		{
			for(jsint i(0); i<ids->length; i++)
			{
				jsval idVal;

				if(	JS_IdToValue(cx, ids->vector[i], &idVal) &&
					JS_ConvertArguments(cx, 1, &idVal, "s", &s) &&
					JS_GetPropertyById(cx, obj, ids->vector[i], &rval) &&
					JS_ConvertArguments(cx, 1, &rval, "b", &b) && b)
				{
					if(!Path::isValidPathPart(s))
					{
						JS_DestroyIdArray(cx, ids);
						throw exception_badPathPart(s);
						return false;
					}

					if(b)
					{
						s_.insert(s);
					}
				}
			}
			JS_DestroyIdArray(cx, ids);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::fromConf_staticInstances(TMStaticInstances &m, jsval conf, const char *from)
	{
		m.clear();

		if(JSVAL_IS_NULL(conf) || JSVAL_IS_VOID(conf))
		{
			return true;
		}

		if(!JSVAL_IS_OBJECT(conf))
		{
			throw(exception(std::string(from) + ": conf for statics must be an object"));
			return false;
		}

		JSContext *cx = ecx()->_jsCx;
		JSObject *obj = JSVAL_TO_OBJECT(conf);
		jsval rval;
		char *sName;
		char *sFile;

		JSIdArray *ids = JS_Enumerate(cx, obj);

		if(ids)
		{
			for(jsint i(0); i<ids->length; i++)
			{
				jsval idVal;

				if(	JS_IdToValue(cx, ids->vector[i], &idVal) &&
					JS_ConvertArguments(cx, 1, &idVal, "s", &sName) &&
					JS_GetPropertyById(cx, obj, ids->vector[i], &rval) &&
					JS_ConvertArguments(cx, 1, &rval, "s", &sFile))
				{
					if(!Path::isValidPathPart(sName))
					{
						JS_DestroyIdArray(cx, ids);
						throw exception_badPathPart(sName);
						return false;
					}

					StaticInstancePtr static_ = mkp(new StaticInstance, ROOTNAME);
					static_->_fileName = _scripter.resolveLibName(sFile, from);
					if(static_->_fileName.empty())
					{
						throw exception_noFile(sFile, from);
						return false;
					}

					m.seq.push_back(m.mk(sName, static_));
				}
			}
			JS_DestroyIdArray(cx, ids);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	StaticInstancePtr Router::fromConf_staticInstance(jsval conf, const char *from)
	{
		char *sFile;
		(JSExceptionReporter)JS_ConvertArguments(ecx()->_jsCx, 1, &conf, "s", &sFile);
		StaticInstancePtr static_ = mkp(new StaticInstance, ROOTNAME);
		static_->_fileName = _scripter.resolveLibName(sFile, from);
		if(static_->_fileName.empty())
		{
			throw exception_noFile(sFile, from);
			return StaticInstancePtr();
		}

		return static_;
	}

	//////////////////////////////////////////////////////////////////////////
	jsval	Router::toConf_pointInstance		(PointInstancePtr point)
	{
		JSContext *cx = ecx()->_jsCx;
		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		//супер
		if(point->getSuper())
		{
			std::string superPath = "/"+point->getSuper()->getPath().string();
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				"super", 
				STRING_TO_JSVAL(JS_NewStringCopyN(cx, superPath.data(), superPath.size())),
				NULL, NULL, JSPROP_ENUMERATE);
		}

		//фильтры с хиденами
		{
			const TMFilterInstances &m = point->getFilters();
			if(!m.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"filters", 
					toConf_filterInstances(m),
					NULL, NULL, JSPROP_ENUMERATE);
			}
			const TSNames &s = point->getFilters_hidden();
			if(!s.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"filters_hidden", 
					toConf_names(s),
					NULL, NULL, JSPROP_ENUMERATE);
			}
		}

		//пропертя с хиденами
		{
			const TMPropertyInstances &m = point->getProperties();
			if(!m.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"properties", 
					toConf_propertyInstances(m),
					NULL, NULL, JSPROP_ENUMERATE);
			}
			const TSNames &s = point->getProperties_hidden();
			if(!s.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"properties_hidden", 
					toConf_names(s),
					NULL, NULL, JSPROP_ENUMERATE);
			}
		}

		//дочерние с хиденами
		{
			const TMPointInstances &m = point->getChilds();
			if(!m.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"childs", 
					toConf_pointInstances(m),
					NULL, NULL, JSPROP_ENUMERATE);
			}
			const TSNames &s = point->getChilds_hidden();
			if(!s.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"childs_hidden", 
					toConf_names(s),
					NULL, NULL, JSPROP_ENUMERATE);
			}
		}

		//статики с хиденами
		{
			const TMStaticInstances &m = point->getStatics();
			if(!m.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"statics", 
					toConf_staticInstances(m),
					NULL, NULL, JSPROP_ENUMERATE);
			}
			const TSNames &s = point->getStatics_hidden();
			if(!s.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"statics_hidden", 
					toConf_names(s),
					NULL, NULL, JSPROP_ENUMERATE);
			}
		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	jsval	Router::toConf_pointInstances		(const TMPointInstances &m)
	{
		JSContext *cx = ecx()->_jsCx;
		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		BOOST_FOREACH(const TMPointInstances::Element &el, m.seq)
		{
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				el.first.c_str(), 
				toConf_pointInstance(el.second),
				NULL, NULL, JSPROP_ENUMERATE);
		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	jsval	Router::toConf_propertyInstance		(PropertyInstancePtr p)
	{
		JSContext *cx = ecx()->_jsCx;
		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		//std::string				_kind;
		if(!p->_kind.empty())
		{
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				"kind", 
				STRING_TO_JSVAL(JS_NewStringCopyN(cx, p->_kind.data(), p->_kind.size())),
				NULL, NULL, JSPROP_ENUMERATE);
		}

		//std::string				_description;
		if(!p->_description.empty())
		{
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				"description", 
				STRING_TO_JSVAL(JS_NewStringCopyN(cx, p->_description.data(), p->_description.size())),
				NULL, NULL, JSPROP_ENUMERATE);
		}

		//bool					_executable;
		//if(p->_executable)
		{
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				"executable", 
				p->_executable?JSVAL_TRUE:JSVAL_FALSE,
				NULL, NULL, JSPROP_ENUMERATE);
		}
		 
		if(p->_file.empty())
		{
			//std::string				_value;
			if(!p->_value.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"value", 
					STRING_TO_JSVAL(JS_NewStringCopyN(cx, p->_value.data(), p->_value.size())),
					NULL, NULL, JSPROP_ENUMERATE);
			}
		}
		else
		{
			//boost::filesystem::path	_file;
			std::string file = p->_file.string();
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				"file", 
				STRING_TO_JSVAL(JS_NewStringCopyN(cx, file.data(), file.size())),
				NULL, NULL, JSPROP_ENUMERATE);

			//bool						_traceFile;
			//if(p->_traceFile)
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"traceFile", 
					p->_traceFile?JSVAL_TRUE:JSVAL_FALSE,
					NULL, NULL, JSPROP_ENUMERATE);
			}

		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	jsval	Router::toConf_propertyInstances	(const TMPropertyInstances &m)
	{
		JSContext *cx = ecx()->_jsCx;
		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		BOOST_FOREACH(const TMPropertyInstances::Element &el, m.seq)
		{
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				el.first.c_str(), 
				toConf_propertyInstance(el.second),
				NULL, NULL, JSPROP_ENUMERATE);
		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	jsval	Router::toConf_filterInstance		(FilterInstancePtr f)
	{
		JSContext *cx = ecx()->_jsCx;
		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		// TMPropertyInstances		_properties;
		// TSNames					_properties_hidden;
		//пропертя с хиденами
		{
			const TMPropertyInstances &m = f->_properties;
			if(!m.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"properties", 
					toConf_propertyInstances(m),
					NULL, NULL, JSPROP_ENUMERATE);
			}
			const TSNames &s = f->_properties_hidden;
			if(!s.empty())
			{
				(JSExceptionReporter)JS_DefineProperty(
					cx, 
					res, 
					"properties_hidden", 
					toConf_names(s),
					NULL, NULL, JSPROP_ENUMERATE);
			}
		}

		// EFilterInstanceKind _kind;
		const char *kind = "";
		switch(f->_kind)
		{
		case efikUnknown:
		default:
			assert(0);
			kind = "unknown";
			break;
		case efikPreTransit:
			kind = "preTransit";
			break;
		case efikPreThis:
			kind = "preThis";
			break;
		case efikPostThis:
			kind = "postThis";
			break;
		case efikPostTransit:
			kind = "postTransit";
			break;
		}
		(JSExceptionReporter)JS_DefineProperty(
			cx, 
			res, 
			"kind", 
			STRING_TO_JSVAL(JS_NewStringCopyZ(cx, kind)),
			NULL, NULL, JSPROP_ENUMERATE);

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	jsval	Router::toConf_filterInstances		(const TMFilterInstances &m)
	{
		JSContext *cx = ecx()->_jsCx;
		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		BOOST_FOREACH(const TMFilterInstances::Element &el, m.seq)
		{
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				el.first.c_str(), 
				toConf_filterInstance(el.second),
				NULL, NULL, JSPROP_ENUMERATE);
		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	jsval	Router::toConf_staticInstance		(StaticInstancePtr s)
	{
		JSContext *cx = ecx()->_jsCx;
		return STRING_TO_JSVAL(JS_NewStringCopyN(cx, s->_fileName.data(), s->_fileName.size()));
	}

	//////////////////////////////////////////////////////////////////////////
	jsval	Router::toConf_staticInstances		(const TMStaticInstances &m)
	{
		JSContext *cx = ecx()->_jsCx;
		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		BOOST_FOREACH(const TMStaticInstances::Element &el, m.seq)
		{
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				el.first.c_str(), 
				toConf_staticInstance(el.second),
				NULL, NULL, JSPROP_ENUMERATE);
		}

		return OBJECT_TO_JSVAL(res);
	}

	//////////////////////////////////////////////////////////////////////////
	jsval	Router::toConf_names				(const TSNames &s)
	{
		JSContext *cx = ecx()->_jsCx;
		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		BOOST_FOREACH(const TSNames::value_type &el, s)
		{
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				res, 
				el.c_str(), 
				JSVAL_TRUE,
				NULL, NULL, JSPROP_ENUMERATE);
		}

		return OBJECT_TO_JSVAL(res);
	}


	//////////////////////////////////////////////////////////////////////////
	void Router::setRootInstance(PointInstancePtr point)
	{
		if(_root)
		{
			_root->deinit();
		}
		_root.reset();
		if(_rootInstance)
		{
			_rootInstance->deinit();
		}
		_rootInstance.reset();
		_rootInstance = point;
		_rootInstance->initPath(Path());
		_rootInstance->init();

		deletePermanent("root");
		_root = createPoint(NULL, _rootInstance, PointInstancePtr(), Path());
		_root->init();
		jsRegisterProp("root", _root->thisJsval());
	}

	//////////////////////////////////////////////////////////////////////////
	PointPtr	Router::createPoint(Point *parent, PointInstancePtr pointInstance, PointInstancePtr superPointInstance, const Path &path)
	{
		PointPtr res = JsObject::mkp(new Point(parent, pointInstance, superPointInstance, path), ROOTNAME);

		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	void Router::structChanged(const Path &where)
	{
		if(_rootInstance)
		{
			_rootInstance->structChanged(where);
		}
		if(_root)
		{
			_root->structChanged(where);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Router::Router()
		: JsObject(false, "Router")
		, _scripter(eomStderrOnly)
		, _config(NULL)
		, _gcNormalBytes(30*1024*1024)
		, _cacheFlushPart(0.1)
		, _cacheAliveTime(1*60)
		, _stackLimit(500000)
#if USE_PROFILER
		, _profilerAccumuleRequests(100)
		, _profilerOrderField(0)
		, _profilerLinesAmount(20)
#endif
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Router::~Router()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::create(const char *fname)
	{
		jsCreate();

		jsval jsv = _scripter.f_execFile(fname);
		if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
		{
			std::cerr<<"Router::setConfig: bad config"<<std::endl;
			return false;
		}

		if(_config)
		{
			JS_RemoveRoot(ecx()->_jsCx, &_config);
			_config = NULL;
		}
		_config = JSVAL_TO_OBJECT(jsv);
		JS_AddRoot(ecx()->_jsCx, &_config);

		std::vector<std::string> vs = getConfigArrayString("scriptDiectories");
		BOOST_FOREACH(const std::string &v, vs)
		{
			_scripter.addLibDir(v.data());
		}

		_documentRoot = getConfigString("staticDiectory");
		if(_documentRoot == "undefined") _documentRoot.clear();

		_i18n = mkp(new I18n(true), ROOTNAME);
		jsRegisterProp("i18n", _i18n->thisJsval());

		_gcNormalBytes = getConfigUlong("memory.gcNormalBytes");
		if(!_gcNormalBytes) _gcNormalBytes = 30*1024*1024;
		_cacheFlushPart = getConfigDouble("memory.cacheFlushPart");
		if(!_cacheFlushPart) _cacheFlushPart = 0.1;
		_cacheAliveTime = getConfigUlong("memory.cacheAliveTime");
		if(!_cacheAliveTime) _cacheAliveTime = 1*60;


		_stackLimit = getConfigUlong("memory.stackLimit");
		if(!_stackLimit) _stackLimit = 500000;


#if USE_PROFILER
		_profilerAccumuledAmount = 0;
		_profilerAccumuleRequests = getConfigUlong("profiler.accumuleRequests");
		_profilerLog.open(getConfigString("profiler.log").c_str());

		std::vector<std::string> fields = getConfigArrayString("profiler.fields");
		for(size_t i(0); i<fields.size(); i++)
		{
			_profilerFields.push_back(parseProfilerField(fields[i].c_str()));
		}
		_profilerOrderField = parseProfilerField(getConfigString("profiler.order").c_str());
		_profilerLinesAmount = getConfigUlong("profiler.linesAmount");
#endif
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	jsval Router::getConfigValue(const char *name, JSObject *obj)
	{
		if(!name || !name[0]) return JSVAL_VOID;
		if(!obj) obj = _config;
		if(!obj) 
		{
			std::cerr<<"Router::getConfigValue: config is not setted"<<std::endl;
			return JSVAL_VOID;
		}

		const char *dot = strchr(name, '.');
		std::string key(name, dot?dot:(name+strlen(name)));
		jsval jsv;
		if(!JS_GetProperty(ecx()->_jsCx, obj, key.data(), &jsv))
		{
			(JSExceptionReporter)false;
			return JSVAL_VOID;
		}

		if(dot)
		{
			if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
			{
				std::cerr<<"Router::getConfigValue: value must be an object: "<<key<<std::endl;
				return JSVAL_VOID;
			}

			std::string subName(dot+1, name+strlen(name));
			return getConfigValue(subName.data(), JSVAL_TO_OBJECT(jsv));
		}

		return jsv;
	}


	//////////////////////////////////////////////////////////////////////////
	std::string Router::getConfigString(const char *name)
	{
		jsval jsv = getConfigValue(name);
		char *v;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &v))
		{
			(JSExceptionReporter)false;
			return "";
		}
		return v;
	}

	//////////////////////////////////////////////////////////////////////////
	unsigned long Router::getConfigUlong(const char *name)
	{
		jsval jsv = getConfigValue(name);
		uint32 v;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "u", &v))
		{
			(JSExceptionReporter)false;
			return 0;
		}
		return v;
	}

	//////////////////////////////////////////////////////////////////////////
	long Router::getConfigLong(const char *name)
	{
		jsval jsv = getConfigValue(name);
		int32 v;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "i", &v))
		{
			(JSExceptionReporter)false;
			return 0;
		}
		return v;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::getConfigBool(const char *name)
	{
		jsval jsv = getConfigValue(name);
		JSBool v;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "b", &v))
		{
			(JSExceptionReporter)false;
			return false;
		}
		return v?true:false;
	}


	//////////////////////////////////////////////////////////////////////////
	double Router::getConfigDouble(const char *name)
	{
		jsval jsv = getConfigValue(name);
		jsdouble v;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "d", &v))
		{
			(JSExceptionReporter)false;
			return 0;
		}
		return v;
	}

	//////////////////////////////////////////////////////////////////////////
	std::map<std::string, std::string> Router::getConfigMapString(const char *name)
	{
		jsval jsv = getConfigValue(name);
		std::map<std::string, std::string> v;
		JSObject *o;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "o", &o))
		{
			(JSExceptionReporter)false;
			return v;
		}
		if(!o)
		{
			std::cerr<<"Router::getConfigMapString: bad config value"<<std::endl;
			return v;
		}

		JSIdArray *ids = JS_Enumerate(ecx()->_jsCx, o);
		for(jsint i(0); i<ids->length; i++)
		{
			jsval kv[2];
			char *key,*val;
			if(	!JS_IdToValue(ecx()->_jsCx, ids->vector[i], &kv[0]) ||
				!JS_GetPropertyById(ecx()->_jsCx, o, ids->vector[i], &kv[1]) ||
				!JS_ConvertArguments(ecx()->_jsCx, 2, kv, "ss", &key, &val))
			{
				(JSExceptionReporter)false;
				JS_DestroyIdArray(ecx()->_jsCx, ids);
				return v;
			}

			v[key] = val;
		}
		JS_DestroyIdArray(ecx()->_jsCx, ids);
		return v;
	}

	//////////////////////////////////////////////////////////////////////////
	std::map<std::string, int> Router::getConfigMapStringInt(const char *name)
	{
		jsval jsv = getConfigValue(name);
		std::map<std::string, int> v;
		JSObject *o;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "o", &o))
		{
			(JSExceptionReporter)false;
			return v;
		}
		if(!o)
		{
			std::cerr<<"Router::getConfigMapString: bad config value"<<std::endl;
			return v;
		}

		JSIdArray *ids = JS_Enumerate(ecx()->_jsCx, o);
		for(jsint i(0); i<ids->length; i++)
		{
			jsval kv[2];
			char *key;
			int32 val;
			if(	!JS_IdToValue(ecx()->_jsCx, ids->vector[i], &kv[0]) ||
				!JS_GetPropertyById(ecx()->_jsCx, o, ids->vector[i], &kv[1]) ||
				!JS_ConvertArguments(ecx()->_jsCx, 2, kv, "si", &key, &val))
			{
				(JSExceptionReporter)false;
				JS_DestroyIdArray(ecx()->_jsCx, ids);
				return v;
			}

			v[key] = val;
		}
		JS_DestroyIdArray(ecx()->_jsCx, ids);
		return v;
	}
	//////////////////////////////////////////////////////////////////////////
	std::vector<std::string> Router::getConfigArrayString(const char *name)
	{
		jsval jsv = getConfigValue(name);
		std::vector<std::string> v;
		JSObject *o;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "o", &o))
		{
			(JSExceptionReporter)false;
			return v;
		}
		if(!o)
		{
			std::cerr<<"Router::getConfigArrayString: bad config value"<<std::endl;
			return v;
		}

		JSIdArray *ids = JS_Enumerate(ecx()->_jsCx, o);
		for(jsint i(0); i<ids->length; i++)
		{
			jsval jsv;
			char *val;
			if(	!JS_GetPropertyById(ecx()->_jsCx, o, ids->vector[i], &jsv) ||
				!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &val))
			{
				(JSExceptionReporter)false;
				JS_DestroyIdArray(ecx()->_jsCx, ids);
				return v;
			}

			v.push_back(val);
		}
		JS_DestroyIdArray(ecx()->_jsCx, ids);
		return v;
	}



	//////////////////////////////////////////////////////////////////////////
	bool Router::initializeThread()
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void Router::deinitializeThread()
	{
	}


	//////////////////////////////////////////////////////////////////////////
	bool Router::probe(Connection4Backend *connection)
	{
		ScripterScopedStackLimit sssl(_stackLimit);

		//сначала отсмотреть статику
		boost::filesystem::path path = (_documentRoot / connection->_requestPath);
		if(boost::filesystem::is_regular_file(path))
		{
			path = boost::filesystem::complete(path).normalize();
			connection->_staticPath = path.string();
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		//резолв пути на структуру
		{
			ScripterScopedRequest ssr;
			//после прошлой сборки мусора почистить неудаленные носители
			_contextDataStorage.flushUnregisterCarriers();

			JSObject *executePlan = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);
			if(!executePlan)
			{
				JSExceptionReporter(false);
				return false;
			}
			JSObject *executePlanData = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
			if(!executePlanData)
			{
				JSExceptionReporter(false);
				return false;
			}

			bool res = true;
			std::string staticPath;
			Request *r(new Request(connection));
			if(!r->initForHeaders())
			{
				JSExceptionReporter(false);
				res = false;
			}

			if(res && !r->jsRegisterProp("plan", executePlan, true))
			{
				(JSExceptionReporter)false;
				res = false;
			}
			if(res && !r->jsRegisterProp("planData", executePlanData, true))
			{
				(JSExceptionReporter)false;
				return false;
			}

			ConnectionData *connectionData = new ConnectionData;
			connectionData->_request = r;
			if(res && !buildExecutePlan(Path(connection->_requestPath), executePlan, executePlanData, res, connection->_staticPath, connectionData->_points))
			{
				delete connectionData;
				JSExceptionReporter(false);
				res = false;
			}

			if(!res)
			{
				connection->_outStatus = 404;
				connection->_outHeaders = "Content-Type: text/html; charset=utf-8\r\n";

				connection->_outBody = ""
					"<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1>"
					"<p>The requested URL " + escxml(connection->_requestPath) + " was not found on this server.</p></body></html>";

				delete connectionData;
				return false;
			}

			if(!connection->_staticPath.empty())
			{
				delete connectionData;
				return true;
			}

			_scripter.jsDefineInGlobal("request", r);
			if(!executeForHeaders(r, connectionData->_points))
			{
				_scripter.jsDefineInGlobal("request");
				delete connectionData;
				return false;
			}
			_scripter.jsDefineInGlobal("request");


			connection->_backendData = connectionData;

			if(connection->_inContentLength)
			{//надо вычитать тело
				return true;
			}

			//тело вычитывать не нужно, обработать сразу
			process(connection);
			return false;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	void Router::cleanup(void *connectionData)
	{
		delete (ConnectionData*)connectionData;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::process(Connection4Backend *connection)
	{
		ScripterScopedStackLimit sssl(_stackLimit);

		ConnectionData *cd = (ConnectionData *)connection->_backendData;
		if(!cd)
		{
			return false;
		}


		Request *r = cd->_request.get();
		if(!r->initForBody())
		{
			(JSExceptionReporter)false;
			cleanup(connection->_backendData);
			connection->_backendData = NULL;
			return false;
		}
		_scripter.jsDefineInGlobal("request", r);

		bool res = true;
		ecx()->_scripter->requestStart(r);
		res = executeForBody(r, cd->_points);
		ecx()->_scripter->requestStop();

		r->out.flush();

		if(res)
		{
			if(!connection->_outStatus)
			{
				connection->_outStatus = 200;
			}
		}
		else
		{
			(JSExceptionReporter)false;
			res = false;
		}

		_scripter.jsDefineInGlobal("request");

		cleanup(connection->_backendData);
		connection->_backendData = NULL;

#if USE_PROFILER
		writeProfilerResult();
#endif
		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	void Router::cronTick(std::ostream &err)
	{
		_crontab->tick();
		_cache->tick();

		time_t now;
		time(&now);
		_cache->delOld(now - _cacheAliveTime);

		_scripter.requestMaybeGc();
		uint32 bytes = JS_GetGCParameter(_scripter._rt, JSGC_BYTES);

		if(bytes > _gcNormalBytes && _cache->size())
		{
			while(bytes > _gcNormalBytes && _cache->size())
			{
				_cache->delOld(_cacheFlushPart);
				_scripter.requestGc();
				bytes = JS_GetGCParameter(_scripter._rt, JSGC_BYTES);
			}
		}
		//std::cerr<<"bytes: "<<bytes<<", nodes: "<<_cache->size()<<std::endl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::preStart()
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void Router::postStart()
	{
		_cache->delOld(1.0);
	}



	//////////////////////////////////////////////////////////////////////////
	bool Router::buildExecutePlan(
		const Path &path, 
		JSObject *executePlan, 
		JSObject *executePlanData, 
		bool &res,
		std::string &staticPath,
		std::vector<PointPtr> &points)
	{
		if(!_root)
		{
			res = false;
			return false;
		}

		jsval jsv;

		int nextIndex = 0;

		//первый элемент плана выполнения - корневая точка
		JSObject *executePlanRootEntry = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		if(!JS_DefineProperty(ecx()->_jsCx, executePlanRootEntry, "point", _root->thisJsval(), NULL, NULL, JSPROP_ENUMERATE)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, executePlanRootEntry, "path", STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "/", 1)), NULL, NULL, JSPROP_ENUMERATE)) return false;
		jsv = OBJECT_TO_JSVAL(executePlanRootEntry);
		if(!JS_SetElement(ecx()->_jsCx, executePlan, nextIndex++, &jsv)) return false;


		//цикл по отрезкам запрошенного пути
		Path accumuledPath;
		Point *lastPoint = _root.get();
		JSObject *lastResObject = executePlanRootEntry;
		points.resize(path.size()+1);
		points[0] = lastPoint;
		//BOOST_FOREACH(std::string &pathPart, r->path)
		for(size_t i(0); i<path.size()+1; i++)
		{
			const std::string staticStub = "";
			const std::string &pathPart = i<path.size()?path[i]:(path.size()?path.back():staticStub);
			if(i<path.size())
			{
				jsval args[2] = {STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, pathPart.data(), pathPart.size())), OBJECT_TO_JSVAL(executePlanData)};
				if(!lastPoint->call_resolve(2, args, &jsv)) return false;
			}
			else
			{
				jsval args[2] = {OBJECT_TO_JSVAL(lastResObject), OBJECT_TO_JSVAL(executePlanData)};
				if(!lastPoint->call_resolveSelf(2, args, &jsv)) return false;
			}

			JSObject *newResObject;

			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "o", &newResObject)) return false;
			if(!newResObject)
			{
				//вернул не объект, резолв не прошел
				res = false;
				return true;
			}

			bool isGoto = false;
			if(!JS_GetProperty(ecx()->_jsCx, newResObject, "goto", &jsv)) return false;
			if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
			{
				isGoto = true;
// 				if(!JS_DefineProperty(
// 					ecx()->_jsCx, 
// 					lastResObject, 
// 					"goto", 
// 					jsv,
// 					NULL, NULL, JSPROP_ENUMERATE)) return false;
// 
// 				if(!JS_DeleteProperty(ecx()->_jsCx, newResObject, "goto")) return false;

			}
			else
			{
				if(!JS_GetProperty(ecx()->_jsCx, newResObject, "point", &jsv)) return false;
			}

			if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
			{
				if(!isGoto && nextIndex == path.size())
				{
					//вдруг статик
					if(!JS_GetProperty(ecx()->_jsCx, newResObject, "static", &jsv)) return false;
					if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
					{
// 						JS_ReportError(ecx()->_jsCx, "Router::buildExecutePlan unable to get Point or Static for /%s", (accumuledPath/pathPart).string().data());
// 						return false;
						res = false;
						return true;
					}
					Static *s = dynamic_cast<Static *>(JsObject::thisObj(JSVAL_TO_OBJECT(jsv)));
					if(!s)
					{
// 						JS_ReportError(ecx()->_jsCx, "Router::buildExecutePlan static object is not a Static for /%s", (accumuledPath/pathPart).string().data());
// 						return false;
						res = false;
						return true;
					}

					//пусть сразу, без повторного выполнения плана
					staticPath = s->getFileName();
					res = true;
					return true;





					if(!JS_DefineProperty(
						ecx()->_jsCx, 
						newResObject, 
						"pathPart", 
						STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, pathPart.data(), pathPart.size())),
						NULL, NULL, JSPROP_ENUMERATE)) return false;

					if(!JS_DefineProperty(
						ecx()->_jsCx, 
						newResObject, 
						"static", 
						s->thisJsval(),
						NULL, NULL, JSPROP_ENUMERATE)) return false;


					std::string apath = "/" + accumuledPath.string() + "/" + pathPart;
					if(!JS_DefineProperty(
						ecx()->_jsCx, 
						newResObject, 
						"path", 
						STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, apath.data(), apath.size())),
						NULL, NULL, JSPROP_ENUMERATE)) return false;

					jsv = OBJECT_TO_JSVAL(newResObject);
					if(!JS_SetElement(
						ecx()->_jsCx, 
						executePlan, 
						i<path.size()?nextIndex:nextIndex-1,
						&jsv))
						return false;







 					res = true;
 					return true;

				}
				else
				{
//	 				JS_ReportError(ecx()->_jsCx, "Router::buildExecutePlan unable to get Point for /%s", (accumuledPath/pathPart).string().data());
// 					return false;
					res = false;
					return true;
				}
			}

			lastPoint = dynamic_cast<Point *>(JsObject::thisObj(JSVAL_TO_OBJECT(jsv)));
			if(!lastPoint)
			{
				JS_ReportError(ecx()->_jsCx, "Router::buildExecutePlan point object is not a Point for /%s", (accumuledPath/pathPart).string().data());
				return false;
			}

			if(!JS_DefineProperty(
				ecx()->_jsCx, 
				newResObject, 
				"pathPart", 
				STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, pathPart.data(), pathPart.size())),
				NULL, NULL, JSPROP_ENUMERATE)) return false;

			if(isGoto)
			{
				if(!JS_DefineProperty(
					ecx()->_jsCx, 
					newResObject, 
					"point", 
					lastPoint->thisJsval(),
					NULL, NULL, JSPROP_ENUMERATE)) return false;
			}
			else
			{
				if(i<path.size())
				{
					accumuledPath.push_back(pathPart);
				}
			}


			std::string apath = "/" + accumuledPath.string();
			if(!JS_DefineProperty(
				ecx()->_jsCx, 
				newResObject, 
				"path", 
				STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, apath.data(), apath.size())),
				NULL, NULL, JSPROP_ENUMERATE)) return false;

			jsv = OBJECT_TO_JSVAL(newResObject);
			size_t planIndex = i<path.size()?nextIndex:nextIndex-1;
			if(!JS_SetElement(
				ecx()->_jsCx, 
				executePlan, 
				planIndex,
				&jsv))
				return false;
			while(planIndex >= points.size()) points.push_back(NULL);
			points[planIndex] = lastPoint;

			if(i<path.size())
			{
				nextIndex++;
			}

			if(isGoto)
			{
				nextIndex--;
				i--;
			}

			lastResObject = newResObject;
		}

		res = true;
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Router::executeForHeaders(Request *r, const std::vector<PointPtr> &points)
	{
		//а тут ниче нету...
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::executeForBody(Request *r, const std::vector<PointPtr> &points)
	{
		//////////////////////////////////
		//фильтры пре-транзитные для всех точек в массиве
		EFilterInstanceProcessResult fpr;
		for(size_t i(0); i<points.size(); i++)
		{
			if(!points[i]->executeFilters(efikPreTransit, fpr)) return false;
			if(efiprComplete == fpr) return true;
		}

		//фильтры пре-собственные для последней точки в массиве
		if(!points.back()->executeFilters(efikPreThis, fpr)) return false;
		if(efiprComplete == fpr) return true;





		//////////////////////////////////
		//дернуть access
		JSBool access = JS_TRUE;
		for(size_t i(0); i<points.size(); i++)
		{
			jsval jsv;
			if(!points[i]->call_access(0, NULL, &jsv)) return false;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "b", &access)) return false;
			if(!access) break;
		}

		if(!access)
		{
			r->_connection->_outStatus = 403;
			r->_connection->_outHeaders = "Content-Type: text/html; charset=utf-8\r\n";

			r->_connection->_outBody = ""
				"<html><head><title>403 Forbidden</title></head><body><h1>Forbidden</h1>"
				"<p>You don't have permission to access " + r->_connection->_requestPath + " on this server.</p></body></html>";
			return true;
		}

		jsval jsv;
		//контроллер последней точки
		if(!points.back()->call_process(0, NULL, &jsv)) return false;

		//фильтры пост-собственные для последней точки в массиве
		if(!points.back()->executeFilters(efikPostThis, fpr)) return false;
		if(efiprComplete == fpr) return true;

		//фильтры пост-транзитные для всех точек в массиве
		for(size_t i(points.size()); i>0; i--)
		{
			if(!points[i-1]->executeFilters(efikPostTransit, fpr)) return false;
			if(efiprComplete == fpr) return true;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	int Router::parseProfilerField(const char *s)
	{
		int field = 0;
		if(strstr(s, "own_")) field |= epmaOwn;
		if(strstr(s, "childs_")) field |= epmaChilds;
		if(strstr(s, "all_")) field |= epmaAll;

		if(strstr(s, "avg_"))
		{
			if(strstr(s, "_usr")) field |= epmkAvgUser;
			if(strstr(s, "_sys")) field |= epmkAvgSystem;
			if(strstr(s, "_real")) field |= epmkAvgReal;
		}
		else
		{
			if(strstr(s, "_usr")) field |= epmkUser;
			if(strstr(s, "_sys")) field |= epmkSystem;
			if(strstr(s, "_real")) field |= epmkReal;
			if(strstr(s, "_calls")) field |= epmkCalls;
		}

		if(strstr(s, "_%")) field |= epmmPercent;
		else field |= epmmUnit;

		return field;
	}

#if USE_PROFILER
	//////////////////////////////////////////////////////////////////////////
	void Router::writeProfilerResult()
	{
		if(_profilerLog)
		{
			_profilerAccumuledAmount++;
			if(!(_profilerAccumuledAmount % _profilerAccumuleRequests))
			{
				time_t t = time(NULL);
				_profilerLog<<"requests: "<<_profilerAccumuledAmount<<"; "<<ctime(&t);
				g_profiler.dumpTable(_profilerLog, " ", _profilerFields.size(), _profilerFields.size()?&_profilerFields[0]:0, _profilerOrderField, _profilerLinesAmount);
				g_profiler.reset();
				_profilerAccumuledAmount = 0;
			}
		}
	}
#endif


	//////////////////////////////////////////////////////////////////////////
	Scripter &Router::getScripter()
	{
		return _scripter;
	}

	//////////////////////////////////////////////////////////////////////////
	PointInstancePtr Router::getRootPointInstance()
	{
		return _rootInstance;
	}
	
	//////////////////////////////////////////////////////////////////////////
	PointPtr Router::getRootPoint()
	{
		return _root;
	}


	//////////////////////////////////////////////////////////////////////////
	void Router::evalPossibleSuperPaths(const Path &inh, TVPaths &supers)
	{
		supers.clear();
		if(_rootInstance)
		{
			evalPossibleSuperPaths_impl(inh, supers, _rootInstance);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::isPossibleSuperPath(const Path &inh, const Path &super)
	{
		if(!_rootInstance)
		{
			//негде наследовать
			return false;
		}

		//экземпляр супера
		PointInstancePtr psuper = _rootInstance->point4Path(super);
		if(!psuper)
		{
			//супера не существует, запретить от него наследовать
			return false;
		}

		//равные начала
		if(inh.eqSmaller(super))
		{
			//короткий есть начало длинного либо равны, запретить наследование
			return false;
		}

		PointInstancePtr pinh = _rootInstance->point4Path(inh);
		if(!pinh)
		{
			//экземпляра к наследованию еще нет, соответственно он не может учавствовать в циклах
			return true;
		}

		if(pinh->hasSuper(psuper))
		{//уже супер
			return true;
		}


		//далее 4 варианта цикла по наследованию/владению
		PointInstancePtr iter;

		//чтобы каждый родитель супера не имел супером потенциального наследника
		iter = psuper;
		while(iter)
		{
			if(iter == pinh)
			{
				//цикл
				return false;
			}
			if(iter->hasSuper(pinh))
			{
				//цикл
				return false;
			}
			iter = iter->getParent();
		}

		//чтобы каждый супер супера не имел родителем потенциального наследника
		iter = psuper;
		while(iter)
		{
			if(iter == pinh)
			{
				//цикл
				return false;
			}
			if(iter->hasParent(pinh))
			{
				//цикл
				return false;
			}
			iter = iter->getSuper();
		}

		//чтобы супер не имел супером никого из родителей потенциального наследника
		iter = pinh;
		while(iter)
		{
			if(iter == psuper)
			{
				//цикл
				return false;
			}
			if(psuper->hasSuper(iter))
			{
				//цикл
				return false;
			}
			iter = iter->getParent();
		}

		//чтобы супер не имел родителем никого из суперов потенциального наследника
		iter = pinh;
		while(iter)
		{
			if(iter == psuper)
			{
				//цикл
				return false;
			}
			if(psuper->hasParent(iter))
			{
				//цикл
				return false;
			}
			iter = iter->getSuper();
		}

		//тяжелая хрень получилась...
		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	void Router::jsCreate()
	{
		ecx_rw()->_router = this;
		ecx_rw()->_scripter = &_scripter;
		ecx_rw()->_request = NULL;
		ecx_rw()->_jsCx = NULL;
		ecx_rw()->_npc = &_newPropsCollector;
		ecx_rw()->_cds = &_contextDataStorage;

		_scripter.start();
		_scripter.requestStart(NULL);

		JsObject::jsCreate();

		_crontab = mkp(new Crontab, ROOTNAME);
		jsRegisterProp("crontab", _crontab->thisJsval());

		_cache = mkp(new Cache, ROOTNAME);
		jsRegisterProp("cache", _cache->thisJsval());
		

		_plugins = mkp(new Plugins, ROOTNAME);
		jsRegisterProp("plugins", _plugins->thisJsval());

		_contextDataStorage.startInstance("global");

		{
			unsigned crc32 = Crc32("#router", 7);

			ContextDataPtr cd = mkp<ContextData>(new ContextData((void *)crc32), ROOTNAME);
			jsRegisterProp("cd", cd->thisJsval());
		}


		jsRegisterMeth("getPointInstance", &Router::call_getPointInstance, 1);
 		jsRegisterMeth("setPointInstance", &Router::call_setPointInstance, 2);

		jsRegisterMeth("getPoint", &Router::call_getPoint, 1);


 		jsRegisterMeth("evalPossibleSuperPaths", &Router::call_evalPossibleSuperPaths, 1);
		jsRegisterMeth("isPossibleSuperPath", &Router::call_isPossibleSuperPath, 2);

 		jsRegisterMeth("createPointInstance", &Router::call_createPointInstance, 1);
		jsRegisterMeth("createFilterInstance", &Router::call_createFilterInstance, 1);
		jsRegisterMeth("createPropertyInstance", &Router::call_createPropertyInstance, 1);
		jsRegisterMeth("createStaticInstance", &Router::call_createStaticInstance, 1);

		jsRegisterMeth("getPointConf", &Router::call_getPointConf, 1);

		jsRegisterMeth("cdStartInstance", &Router::call_cdStartInstance, 1);
		jsRegisterMeth("cdStopInstance", &Router::call_cdStopInstance, 1);
		jsRegisterMeth("cdGetInstance", &Router::call_cdGetInstance, 1);
		jsRegisterMeth("cdSetInstance", &Router::call_cdSetInstance, 2);

		jsRegisterMeth("createService", &Router::call_createService, 1);
		jsRegisterMeth("createTemplate", &Router::call_createTemplate, 0);
		
		jsRegisterMeth("getConfig", &Router::call_getConfig, 1);
		

		_scripter.jsDefineInGlobal("router", mkp<Router>(this, ROOTNAME));


		_scripter.jsDefineInGlobal("Magick", mkp(new magick::Magick, ROOTNAME));
		_scripter.jsDefineInGlobal("Mime", mkp(new mime::Mime, ROOTNAME));
		_scripter.jsDefineInGlobal("Net", mkp(new net::Net, ROOTNAME));
		_scripter.jsDefineInGlobal("hash", mkp(new Hash, ROOTNAME));

		_scripter.requestStop();
	}

	//////////////////////////////////////////////////////////////////////////
	void Router::jsDestroy()
	{
		_scripter.requestStart(NULL);


		_delayedPointInstanceBuilds.clear();
		_topRoot4DelayedPointInstanceBuilds.reset();

		if(_root)
		{
			_root->deinit();
			_root.reset();
		}

		if(_rootInstance)
		{
			_rootInstance->deinit();
			_rootInstance.reset();
		}

		_crontab.reset();
		deletePermanent("crontab");

		_cache.reset();
		deletePermanent("cache");

		_plugins.reset();
		deletePermanent("plugins");

		_i18n.reset();
		deletePermanent("i18n");

		_scripter.jsDefineInGlobal("router");

		getPropertyPtr<ContextData>("cd")->unregister();
		deletePermanent("cd");

		_contextDataStorage.stopInstance("global");
		_contextDataStorage.reset();

		JsObject::jsDestroy();
		_newPropsCollector.deleteAll();

		if(_config)
		{
			JS_RemoveRoot(ecx()->_jsCx, &_config);
			_config = NULL;
		}

		_scripter.requestStop();
		_scripter.stop();

		ecx_rw()->_router = NULL;
		ecx_rw()->_scripter = NULL;
		ecx_rw()->_request = NULL;
		ecx_rw()->_jsCx = NULL;
		ecx_rw()->_npc = NULL;
		ecx_rw()->_cds = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	void Router::vdeleterJs()
	{
		//nothing
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_getPointInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(!_rootInstance)
		{
			*rval = JSVAL_NULL;
			return true;
		}

		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.getPoint must be called with 1 argument]");
			return false;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &s))return false;

		PointInstancePtr res = _rootInstance->point4Path(s);

		if(res)
		{
			*rval = res->thisJsval();
		}
		else
		{
			*rval = JSVAL_NULL;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_setPointInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 2)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.setPointInstance must be called with 2 arguments]");
			return false;
		}

		char *s;
		JSObject *obj;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "so", &s, &obj))return false;

		if(!obj)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.setPointInstance second arg must be an object]");
			return false;
		}

		PointInstancePtr pointInstance = mkp<PointInstance>(obj, ROOTNAME);
		if(!pointInstance)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.setPointInstance second arg must be an PointInstance]");
			return false;
		}

		Path path(s);
		if(path.empty())
		{
			setRootInstance(pointInstance);
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "[Router.setPointInstance at arbitrary path not implemented yet]");
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_getPoint(uintN argc, jsval *argv, jsval *rval)
	{
		if(!_root)
		{
			*rval = JSVAL_NULL;
			return true;
		}

		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.getPoint must be called with 1 argument]");
			return false;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &s))return false;

		PointPtr res = _root->point4Path(s);

		if(res)
		{
			*rval = res->thisJsval();
		}
		else
		{
			*rval = JSVAL_VOID;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_evalPossibleSuperPaths(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.getPoint must be called with 1 argument]");
			return false;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &s))return false;

		Path path = s;

		TVPaths paths;
		evalPossibleSuperPaths(path, paths);

		std::string str;
		std::vector<jsval> arr(paths.size());
		for(size_t i(0); i<arr.size(); i++)
		{
			str = "/"+paths[i].string();
			arr[i] = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, str.c_str(), str.size()));
		}

		if(arr.size())
		{
			*rval = OBJECT_TO_JSVAL(JS_NewArrayObject(ecx()->_jsCx, arr.size(), &arr[0]));
		}
		else
		{
			*rval = OBJECT_TO_JSVAL(JS_NewArrayObject(ecx()->_jsCx, 0, NULL));
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_isPossibleSuperPath(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 2)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.isPossibleSuperPath must be called with 2 arguments]");
			return false;
		}

		char *inh;
		char *super;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &inh, &super))return false;

		if(isPossibleSuperPath(inh, super))
		{
			*rval = JSVAL_TRUE;
		}
		else
		{
			*rval = JSVAL_FALSE;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_createPointInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createPointInstance must be called with 1 argument]");
			return false;
		}

		bool res = false;
		try
		{
			std::string curFile;
			_scripter.evalCurPosition(&curFile);
			PointInstancePtr point = fromConf_pointInstance(argv[0], curFile.c_str());

			if(point)
			{
				*rval = point->thisJsval();
			}
			else
			{
				*rval = JSVAL_VOID;
			}

			res = true;
		}
		catch (ccms::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createPointInstance ccms exception: %s", e.what());
		}
		catch (std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createPointInstance std exception: %s", e.what());
		}
		catch (...)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createPointInstance unknown exception");
		}

		_delayedPointInstanceBuilds.clear();
		_topRoot4DelayedPointInstanceBuilds.reset();

		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_createFilterInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createFilterInstance must be called with 1 argument]");
			return false;
		}

		try
		{
			std::string curFile;
			_scripter.evalCurPosition(&curFile);
			FilterInstancePtr filter = fromConf_filterInstance(argv[0], curFile.data());

			if(filter)
			{
				*rval = filter->thisJsval();
			}
			else
			{
				*rval = JSVAL_VOID;
			}
		}
		catch (ccms::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createPointInstance ccms exception: %s", e.what());
			return false;
		}
		catch (std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createPointInstance std exception: %s", e.what());
			return false;
		}
		catch (...)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createPointInstance unknown exception");
			return false;
		}

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	bool Router::call_createPropertyInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createPropertyInstance must be called with 1 argument]");
			return false;
		}

		PropertyInstancePtr property = fromConf_propertyInstance(argv[0], NULL);

		if(property)
		{
			*rval = property->thisJsval();
		}
		else
		{
			*rval = JSVAL_VOID;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_createStaticInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createStaticInstance must be called with 1 argument]");
			return false;
		}

		try
		{
			char *from = NULL;
			if(argc>1)
			{
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "s", &from))return false;
			}
			StaticInstancePtr static_ = fromConf_staticInstance(argv[0], from);

			if(static_)
			{
				*rval = static_->thisJsval();
			}
			else
			{
				*rval = JSVAL_VOID;
			}
		}
		catch(std::exception &e)
		{
			*rval = JSVAL_VOID;
			JS_ReportError(ecx()->_jsCx, "[Router.createStaticInstance exception: %s]", e.what());
			return false;
		}
		catch(...)
		{
			*rval = JSVAL_VOID;
			JS_ReportError(ecx()->_jsCx, "[Router.createStaticInstance unknown exception]");
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_getPointConf(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "router.getPointConf must be called with 1 arg");
			return false;
		}

		if(!JSVAL_IS_OBJECT(argv[0]) || JSVAL_IS_NULL(argv[0]))
		{
			JS_ReportError(ecx()->_jsCx, "router.getPointConf must be called with arg-object");
			return false;
		}
		JSObject *obj = JSVAL_TO_OBJECT(argv[0]);

		PointInstancePtr pointInstance = mkp<PointInstance>(obj, ROOTNAME);
		if(!pointInstance)
		{
			PointPtr point = mkp<Point>(obj, ROOTNAME);
			if(!point)
			{
				JS_ReportError(ecx()->_jsCx, "router.getPointConf must be called with arg-point or arg-pointInstance");
				return false;
			}

			pointInstance = point->getInstance();
			if(!pointInstance)
			{
				JS_ReportError(ecx()->_jsCx, "router.getPointConf arg-point is invalid");
				return false;
			}
		}

		*rval = toConf_pointInstance(pointInstance);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_cdStartInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.cdStartInstance must be called with 1 argument]");
			return false;
		}
		char *key;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &key))return false;
		*rval = _contextDataStorage.startInstance(key)?JSVAL_TRUE:JSVAL_FALSE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_cdStopInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.cdStartInstance must be called with 1 argument]");
			return false;
		}
		char *key;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &key))return false;
		*rval = _contextDataStorage.stopInstance(key)?JSVAL_TRUE:JSVAL_FALSE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_cdGetInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.cdGetInstance must be called with 1 argument]");
			return false;
		}
		char *key;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &key))return false;
		*rval = _contextDataStorage.getInstance(key);
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Router::call_cdSetInstance(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 2)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.cdSetInstance must be called with 2 argument]");
			return false;
		}
		char *key;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &key))return false;
		*rval = _contextDataStorage.setInstance(key, argv[1])?JSVAL_TRUE:JSVAL_FALSE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_createService(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Router.createService must be called with 1 argument]");
			return false;
		}
		char *name;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &name))return false;

		if(!strcmp(name, "sqlite3"))
		{
			*rval = mkp(new Sqlite3, ROOTNAME)->thisJsval();
			return true;
		}

		if(!strcmp(name, "pg"))
		{
			*rval = mkp(new Pg, ROOTNAME)->thisJsval();
			return true;
		}

		if(!strcmp(name, "ui"))
		{
			*rval = mkp(new Ui, ROOTNAME)->thisJsval();
			return true;
		}

		if(!strcmp(name, "log"))
		{
			*rval = mkp(new Log, ROOTNAME)->thisJsval();
			return true;
		}

		*rval = JSVAL_VOID;
		JS_ReportError(ecx()->_jsCx, "[Router.createService unknown service %s]", name);
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Router::call_createTemplate(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = mkp(new Template, ROOTNAME)->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Router::call_getConfig(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			char *s;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &s))return false;
			*rval = getConfigValue(s);
			return true;
		}
		
		*rval = OBJECT_TO_JSVAL(_config);
		return true;
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	void Router::evalPossibleSuperPaths_impl(const Path &inh, TVPaths &supers, const PointInstancePtr &iter)
	{
		const Path &iterPath = iter->getPath();
		bool processChilds = true;
		if(inh.eqSmaller(iterPath))
		{//начала совпадают
			if(inh.size()<=iterPath.size())
			{
				//у итера путь длинее, не процессировать дочерних
				processChilds = false;
			}
		}
		else
		{//начала не совпадают
			if(isPossibleSuperPath(inh, iterPath))
			{//отсечены циклы
				supers.push_back(iterPath);
			}
		}

		if(processChilds)
		{
			const TMPointInstances &childs = iter->getChilds();
			TMPointInstances::const_iterator it = childs.begin();
			TMPointInstances::const_iterator en = childs.end();

			for(; it!=en; it++)
			{
				evalPossibleSuperPaths_impl(inh, supers, it->second);
			}
		}
	}

}
