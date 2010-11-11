#include "stdafx.h"
#include "router/plugins.hpp"
#include "scripter/scripter.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Plugins::Plugins()
		: JsObject(true, "[object Plugins]")
	{
		JSObject *obj = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);
		
// 		jsval jsv = STRING_TO_JSVAL(JS_NewStringCopyZ(ecx()->_jsCx, "../plugins"));
// 		JS_SetElement(ecx()->_jsCx, obj, 0, &jsv);

		jsRegisterProp("directories", OBJECT_TO_JSVAL(obj), true);
		jsRegisterMeth("reload", &Plugins::call_reload, 0);

		jsRegisterMeth("serialize", &Plugins::call_serialize, 0);
		jsRegisterMeth("deserialize", &Plugins::call_deserialize, 1);

// 		if(!call_reload(0, NULL, NULL))
// 		{
// 			if(JS_IsExceptionPending(ecx()->_jsCx))
// 			{
// 				JS_ReportPendingException(ecx()->_jsCx);
// 				JS_ClearPendingException(ecx()->_jsCx);
// 			}
// 		}

		//в прототипе будут сами плуги
		//setPrototype(mkp(new JsObject, ROOTNAME));
		JS_SetPrototype(ecx()->_jsCx, thisJsobj(), JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL));
	}

	//////////////////////////////////////////////////////////////////////////
	Plugins::~Plugins()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool Plugins::call_reload(uintN argc, jsval *argv, jsval *rval)
	{
		jsval jsvd;
		if(!JS_GetProperty(ecx()->_jsCx, thisJsobj(), "directories", &jsvd))
		{
			return false;
		}
		if(!JSVAL_IS_OBJECT(jsvd))
		{
			//не ошибка?
			return true;
		}
		JSObject *objd = JSVAL_TO_OBJECT(jsvd);
		if(!objd)
		{
			//не ошибка?
			return true;
		}

		std::string source;
		ecx()->_scripter->evalCurPosition(&source);

		std::vector<std::string> dirs;
		JSIdArray *arr = JS_Enumerate(ecx()->_jsCx, objd);
		for(jsint i(0); i<arr->length; i++)
		{
			jsval jsv = JSVAL_VOID;
			if(!JS_GetPropertyById(ecx()->_jsCx, objd, arr->vector[i], &jsv))return false;

			char *s = NULL;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &s))return false;
			if(s && s[0])
			{
				std::string dirname = ecx()->_scripter->resolveLibName(s, source.c_str(), true);
				if(!dirname.empty())
				{
					dirs.push_back(dirname);
				}
			}
		}
		JS_DestroyIdArray(ecx()->_jsCx, arr);
		reload(dirs);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Plugins::call_serialize(uintN argc, jsval *argv, jsval *rval)
	{
		JSContext *cx = ecx()->_jsCx;
		JSObject *res = JS_NewObject(cx, NULL, NULL, NULL);

		//directories
		jsval jsvd;
		if(!JS_GetProperty(cx, thisJsobj(), "directories", &jsvd))
		{
			return false;
		}

		if(!JS_DefineProperty(
			cx, 
			res, 
			"directories", 
			jsvd,
			NULL, NULL, JSPROP_ENUMERATE))return false;



		//плуги из прототипа
		JSObject *plugs = JS_NewObject(cx, NULL, NULL, NULL);
		JSObject *proto = JS_GetPrototype(cx, thisJsobj());
		JSIdArray *arr = JS_Enumerate(cx, proto);
		for(jsint i(0); i<arr->length; i++)
		{
 			jsval jsv;
			if(!JS_GetPropertyById(cx, proto, arr->vector[i], &jsv))return false;

			JSObject *objPlug = JSVAL_TO_OBJECT(jsv);
			if(objPlug)
			{
				jsv = JSVAL_VOID;
				if(!JS_GetProperty(cx, objPlug, "serialize", &jsv))return false;

				if(JSVAL_IS_OBJECT(jsv))
				{
					if(JS_CallFunctionValue(cx, objPlug, jsv, 0, NULL, &jsv))
					{
						if(!JS_DefinePropertyById(
							cx,
							plugs,
							arr->vector[i],
							jsv,
							NULL, NULL, JSPROP_ENUMERATE))return false;
					}
					else
					{
						JS_DestroyIdArray(cx, arr);
						return false;
					}
				}
				else
				{
					//no method "serialize", its ok
				}
			}
			else
			{
				//plug is not a object
			}
		}
		JS_DestroyIdArray(cx, arr);

		if(!JS_DefineProperty(
			cx, 
			res, 
			"plugins", 
			OBJECT_TO_JSVAL(plugs),
			NULL, NULL, JSPROP_ENUMERATE))return false;

		*rval = OBJECT_TO_JSVAL(res);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Plugins::call_deserialize(uintN argc, jsval *argv, jsval *rval)
	{
		JSContext *cx = ecx()->_jsCx;
		if(1 != argc)
		{
			JS_ReportError(cx, "i118n.deserialize must be called with 1 arg");
			return false;
		}
		JSObject *obj = JSVAL_TO_OBJECT(argv[0]);
		if(!obj)
		{
			JS_ReportError(cx, "i118n.deserialize arg must be an object");
			return false;
		}

		jsval jsv;

		//directories
		if(!JS_GetProperty(cx, obj, "directories", &jsv))return false;
		deletePermanent("directories");
		jsRegisterProp("directories", jsv, true);

		if(!call_reload(0, NULL, &jsv))
		{
			return false;
		}

		//плуги
		JSObject *proto = JS_GetPrototype(cx, thisJsobj());
		if(!JS_GetProperty(cx, obj, "plugins", &jsv))return false;
		JSObject *confs = JSVAL_TO_OBJECT(jsv);
		for(int iter(0); ; iter++)
		{
			bool wasDeserialized = false;
			JSIdArray *arr = JS_Enumerate(cx, confs);
			if(!arr->length)
			{
				//all done
				break;
			}
			for(jsint i(0); i<arr->length; i++)
			{
				jsval jsvConf;
				if(!JS_GetPropertyById(cx, confs, arr->vector[i], &jsvConf))return false;
				jsval jsvPlug;
				if(!JS_GetPropertyById(cx, proto, arr->vector[i], &jsvPlug))return false;

				JSObject *objPlug = NULL;
				if(JSVAL_IS_OBJECT(jsvPlug)) objPlug = JSVAL_TO_OBJECT(jsvPlug);
				if(objPlug)
				{
					jsv = JSVAL_VOID;
					if(!JS_GetProperty(cx, objPlug, "deserialize", &jsv))return false;

					if(JSVAL_IS_OBJECT(jsv))
					{
						jsval args[2] = {jsvConf, INT_TO_JSVAL(iter)};
						if(!JS_CallFunctionValue(cx, objPlug, jsv, 2, args, &jsv))
						{
							if(JS_IsExceptionPending(cx))
							{
								JS_ReportPendingException(cx);
								JS_ClearPendingException(cx);
							}

							JS_DestroyIdArray(cx, arr);
							return false;
						}
						if(JS_IsExceptionPending(cx))
						{
							JS_ReportPendingException(cx);
							JS_ClearPendingException(cx);
						}

						JSBool b;
						if(!JS_ConvertArguments(cx, 1, &jsv, "b", &b))return false;
						if(b)
						{
							wasDeserialized = true;
							if(!JS_DeletePropertyById(cx, confs, arr->vector[i]))return false;
						}
					}
					else
					{
						//no method "deserialize", its ok
						wasDeserialized = true;
						if(!JS_DeletePropertyById(cx, confs, arr->vector[i]))return false;
					}
				}
				else
				{
					//plug is not a object
					wasDeserialized = true;
					if(!JS_DeletePropertyById(cx, confs, arr->vector[i]))return false;
				}
			}
			JS_DestroyIdArray(cx, arr);

			if(!wasDeserialized)
			{
				if(JS_IsExceptionPending(cx))
				{
					JS_ReportPendingException(cx);
					JS_ClearPendingException(cx);
				}

				JS_ReportError(cx, "plugins.deserialize infity loop");
				return false;
			}
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	void Plugins::reload(const std::vector<std::string> &dirs)
	{
		JSContext *cx = ecx()->_jsCx;
		std::vector<std::string> confs;

		//перебрать все директории спугами, и в каждой перебрать директории плугов, оставить только те в которых есть conf.js
		using namespace boost::filesystem;
		BOOST_FOREACH(const std::string &dir, dirs)
		{
			directory_iterator iter(dir), end;
			for(; iter!=end; iter++)
			{
				path dir = *iter;
				path conf;
				if(is_regular_file(conf = dir/"conf.js"))
				{
					confs.push_back(conf.string());
				}
			}
		}

		//грузить конфы поштучно
		typedef std::map<std::string, jsval> TMName2Conf;
		TMName2Conf name2conf;//имя плуга->конф
		BOOST_FOREACH(const std::string &confname, confs)
		{
			jsval conf = ecx()->_scripter->f_execFile(confname);
			if(JSVAL_IS_OBJECT(conf) && !JSVAL_IS_NULL(conf))
			{
				jsval jsvName;
				if(JS_GetProperty(cx, JSVAL_TO_OBJECT(conf), "name", &jsvName) && !JSVAL_IS_VOID(jsvName))
				{

					char *name;
					(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsvName, "s", &name);
					name2conf[name] = conf;
				}
			}
		}

		JSObject *proto = JS_GetPrototype(cx, thisJsobj());
		if(!proto)
		{
			//в прототипе будут сами плуги
			//setPrototype(mkp(new JsObject, ROOTNAME));
			(JSExceptionReporter)JS_SetPrototype(ecx()->_jsCx, thisJsobj(), JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL));
			proto = JS_GetPrototype(cx, thisJsobj());
		}

		//теперь срезать все текущие неинсталированные плуги
		JSIdArray *arr = JS_Enumerate(cx, proto);
		for(jsint i(0); i<arr->length; i++)
		{
			bool doErase = false;
// 			jsval jsvName;
// 			JS_IdToValue(cx, arr->vector[i], &jsvName);
// 			char *name;
// 			JS_ConvertArguments(cx, 1, &jsvName, "s", &name);

			do 
			{
				jsval jsvPlug;
				(JSExceptionReporter)JS_GetPropertyById(cx, proto, arr->vector[i], &jsvPlug);
				if(!JSVAL_IS_OBJECT(jsvPlug) || JSVAL_IS_NULL(jsvPlug))
				{
					doErase = true;
					break;
				}
				JSObject *objPlug = JSVAL_TO_OBJECT(jsvPlug);
				jsval jsvInstalled;
				(JSExceptionReporter)JS_GetProperty(cx, objPlug, "isInstalled", &jsvInstalled);

				JSBool b;
				(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsvInstalled, "b", &b);

				if(!b)
				{
					doErase = true;
					break;
				}
			} while (false);

			if(doErase)
			{
				(JSExceptionReporter)JS_DeletePropertyById(cx, proto, arr->vector[i]);
			}
		}
		JS_DestroyIdArray(cx, arr);

		//добавить новые среди которых нет одноименных в проинсталированных
		BOOST_FOREACH(TMName2Conf::value_type &pair, name2conf)
		{
			JSBool found;
			(JSExceptionReporter)JS_HasProperty(cx, proto, pair.first.c_str(), &found);
			if(found)
			{
				continue;
			}

			//добавить
			(JSExceptionReporter)JS_DefineProperty(
				cx, 
				proto,
				pair.first.c_str(), 
				pair.second, 
				NULL, NULL, JSPROP_ENUMERATE);
			//jsRegisterProp(pair.first.c_str(), pair.second);
		}
	}

}
