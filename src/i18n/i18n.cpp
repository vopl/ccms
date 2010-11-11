#include <stdafx.h>
#include "i18n/i18n.hpp"
#include "i18n/l10n.hpp"
#include "scripter/scripter.hpp"
#include "router/router.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	I18n::I18n(bool collectUsage)
		: JsObject(true, "[object I18n]")
		, _collectUsage(collectUsage)
		, _confs(JSVAL_VOID)
	{
		(JSExceptionReporter)JS_AddNamedRoot(ecx()->_jsCx, &_confs, "I18n::_confs");

		JSObject *obj = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);

// 		jsval jsv = STRING_TO_JSVAL(JS_NewStringCopyZ(ecx()->_jsCx, "i18n"));
// 		JS_SetElement(ecx()->_jsCx, obj, 0, &jsv);

		jsRegisterProp("directories", OBJECT_TO_JSVAL(obj), true);

		jsRegisterMeth("reload", &I18n::call_reload, 0);
		jsRegisterMeth("createL10n", &I18n::call_createL10n, 1);
		jsRegisterMeth("addTranslation", &I18n::call_addTranslation, 2);
		jsRegisterMeth("delTranslation", &I18n::call_delTranslation, 2);
		jsRegisterMeth("dumpUsage", &I18n::call_dumpUsage, 0);

		jsRegisterProp("confs", &I18n::xetter_confs, true);

		//где конфиг?
		std::string tzdbFile = ecx()->_router->getConfigString("timeZoneSpec");
		try
		{
			_tzDatabase.load_from_file(tzdbFile);
		}
		catch(...)
		{
			std::cerr<<"failed to load timezone database from "<<tzdbFile<<std::endl;
		}

		jsRegisterMeth("serialize", &I18n::call_serialize, 0);
		jsRegisterMeth("deserialize", &I18n::call_deserialize, 1);
		

	}

	//////////////////////////////////////////////////////////////////////////
	I18n::~I18n()
	{
		clean2void(_confs);
		(JSExceptionReporter)JS_RemoveRoot(ecx()->_jsCx, &_confs);
	}

	//////////////////////////////////////////////////////////////////////////
	boost::local_time::tz_database I18n::getTzDatabase()
	{
		return _tzDatabase;
	}

	//////////////////////////////////////////////////////////////////////////
	DicTableBucketPtr I18n::mkDicTableBucket(const std::set<std::string> &files, const std::string &language)
	{
		DicTableBucketPtr res(new DicTableBucket);

		BOOST_FOREACH(const std::string &f, files)
		{
			if(_dicTables.end() == _dicTables.find(f))
			{
				DicTablePtr dt(new DicTable);
				dt->load(f);
				_dicTables[f] = dt;
			}
			res->add(_dicTables[f]);
		}

		TMTranslationFiles::iterator miter = _translationFiles.find(language);
		if(_translationFiles.end() != miter)
		{
			BOOST_FOREACH(const std::string &f, miter->second)
			{
				if(_dicTables.end() == _dicTables.find(f))
				{
					DicTablePtr dt(new DicTable);
					dt->load(f);
					_dicTables[f] = dt;
				}
				res->add(_dicTables[f]);
			}
		}

		return res;
	}


	//////////////////////////////////////////////////////////////////////////
	bool I18n::call_reload(uintN argc, jsval *argv, jsval *rval)
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
	bool I18n::xetter_confs(jsval *vp, bool isGet)
	{
		if(JSVAL_IS_VOID(_confs))
		{
			jsval rval;
			if(!call_reload(0, 0, &rval))
			{
				return false;
			}
		}
		*vp = _confs;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool I18n::call_createL10n(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[I18n.createL10n must be called with 1 argument]");
			return false;
		}

		jsval conf = JSVAL_VOID;
		if(JSVAL_IS_OBJECT(argv[0]) && !JSVAL_IS_NULL(argv[0]))
		{
			conf = argv[0];
		}
		else
		{
			char *name;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &name))return false;

			if(JSVAL_IS_VOID(_confs))
			{
				jsval rval;
				if(!call_reload(0, 0, &rval))
				{
					return false;
				}
			}

			if(!JS_GetProperty(ecx()->_jsCx, JSVAL_TO_OBJECT(_confs), name, &conf))return false;

			if(JSVAL_IS_VOID(conf))
			{
				JS_ReportError(ecx()->_jsCx, "[I18n.createL10n conf with name %s not found]", name);
				return false;
			}

		}

		L10nPtr l10n = mkp(new L10n(mkp(this, ROOTNAME), conf), ROOTNAME);
		*rval = l10n->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool I18n::call_addTranslation(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 2)
		{
			JS_ReportError(ecx()->_jsCx, "[I18n.addTranslation must be called with 2 arguments]");
			return false;
		}

		char *lang, *file;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &lang, &file)) return false;

		_translationFiles[lang].insert(file);

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool I18n::call_delTranslation(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 2)
		{
			JS_ReportError(ecx()->_jsCx, "[I18n.delTranslation must be called with 2 arguments]");
			return false;
		}

		char *lang, *file;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &lang, &file)) return false;

		
		TMTranslationFiles::iterator miter = _translationFiles.find(lang);
		if(_translationFiles.end() != miter)
		{
			std::set<std::string>::iterator siter = miter->second.find(file);
			if(miter->second.end() != siter)
			{
				miter->second.erase(siter);
				if(miter->second.empty())
				{
					_translationFiles.erase(miter);
				}
			}
		}

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool I18n::call_dumpUsage(uintN argc, jsval *argv, jsval *rval)
	{
		JS_ReportError(ecx()->_jsCx, "[i18n.dumpUsage not impl]");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	void I18n::reload(const std::vector<std::string> &dirs)
	{
		JSContext *cx = ecx()->_jsCx;
		clean2void(_confs);

		JSObject *confsObj = JS_NewObject(cx, NULL, NULL, NULL);
		_confs = OBJECT_TO_JSVAL(confsObj);

		std::vector<std::string> confs;

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
					(JSExceptionReporter)JS_DefineProperty(cx, confsObj, name, conf, NULL, NULL, JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool I18n::call_serialize(uintN argc, jsval *argv, jsval *rval)
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


		//confs
		if(!JS_DefineProperty(
			cx, 
			res, 
			"confs", 
			_confs,
			NULL, NULL, JSPROP_ENUMERATE))return false;

		//collectUsage
		if(!JS_DefineProperty(
			cx, 
			res, 
			"collectUsage", 
			_collectUsage?JSVAL_TRUE:JSVAL_FALSE,
			NULL, NULL, JSPROP_ENUMERATE))return false;

		//translation files
		{
			JSObject *tfs = JS_NewObject(cx, NULL, NULL, NULL);

			BOOST_FOREACH(const TMTranslationFiles::value_type &mv, _translationFiles)
			{

				JSObject *tfsl = JS_NewObject(cx, NULL, NULL, NULL);
				BOOST_FOREACH(const TSStrings::value_type &sv, mv.second)
				{
					if(!JS_DefineProperty(
						cx, 
						tfsl, 
						sv.data(), 
						JSVAL_TRUE,
						NULL, NULL, JSPROP_ENUMERATE))return false;
				}
				if(!JS_DefineProperty(
					cx, 
					tfs, 
					mv.first.data(), 
					OBJECT_TO_JSVAL(tfsl),
					NULL, NULL, JSPROP_ENUMERATE))return false;
			}
			if(!JS_DefineProperty(
				cx, 
				res, 
				"translationFiles", 
				OBJECT_TO_JSVAL(tfs),
				NULL, NULL, JSPROP_ENUMERATE))return false;

		}

		*rval = OBJECT_TO_JSVAL(res);
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool I18n::call_deserialize(uintN argc, jsval *argv, jsval *rval)
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

		//confs
		if(!JS_GetProperty(cx, obj, "confs", &jsv))return false;
		_confs = jsv;

		//collectUsage
		if(!JS_GetProperty(cx, obj, "collectUsage", &jsv))return false;
		JSBool b;
		if(!JS_ConvertArguments(cx, 1, &jsv, "b", &b))return false;
		_collectUsage = b?true:false;

		//translation files
		{
			_translationFiles.clear();
			if(!JS_GetProperty(cx, obj, "translationFiles", &jsv))return false;
			if(JSVAL_IS_OBJECT(jsv) || !JSVAL_IS_NULL(jsv))
			{
				JSObject *langsObj = JSVAL_TO_OBJECT(jsv);
				JSIdArray *langs = JS_Enumerate(cx, langsObj);

				for(jsint li(0); li < langs->length; li++)
				{
					if(!JS_IdToValue(cx, langs->vector[li], &jsv))return false;
					char *lang;
					if(!JS_ConvertArguments(cx, 1, &jsv, "s", &lang))return false;

					if(!JS_GetPropertyById(cx, langsObj, langs->vector[li], &jsv)) return false;
					if(JSVAL_IS_OBJECT(jsv) || !JSVAL_IS_NULL(jsv))
					{

						JSObject *filesObj = JSVAL_TO_OBJECT(jsv);
						JSIdArray *files = JS_Enumerate(cx, filesObj);

						for(jsint lf(0); lf<files->length; lf++)
						{
							if(!JS_IdToValue(cx, files->vector[lf], &jsv))return false;
							char *file;
							if(!JS_ConvertArguments(cx, 1, &jsv, "s", &file))return false;

							_translationFiles[lang].insert(file);
						}

						JS_DestroyIdArray(cx, files);
					}
				}

				JS_DestroyIdArray(cx, langs);
			}

		}
		*rval = JSVAL_VOID;
		return true;
		//return call_reload(0, NULL, rval);
	}

}
