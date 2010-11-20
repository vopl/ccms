#include "stdafx.h"
#include "scripter/scripter.hpp"
#include "router/request.hpp"

namespace ccms
{
	extern ExecContext *ecx_rw();

	ScopedStateStreams::ScopedStateStreams(std::ostream *out, std::ostream *err, const char *currentName)
		: _out(ecx()->_out)
		, _err(ecx()->_err)
		, _currentName(ecx()->_currentName)
	{
		if(out)
		{
			ecx_rw()->_out = out;
		}
		if(err)
		{
			ecx_rw()->_err = err;
		}
		if(currentName)
		{
			ecx_rw()->_currentName = currentName;
		}
	}

	ScopedStateStreams::~ScopedStateStreams()
	{
		ecx_rw()->_out = _out;
		ecx_rw()->_err = _err;
		ecx_rw()->_currentName = _currentName;
	}



	/* The class of the global object. */
	static JSClass global_class = {
		"global", JSCLASS_GLOBAL_FLAGS,
		JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
		JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
		JSCLASS_NO_OPTIONAL_MEMBERS
	};

	//////////////////////////////////////////////////////////////////////////
	void Scripter::jsf_report_err(JSContext *cx, const char *message, JSErrorReport *report)
	{
		(*ecx()->_err)
			//<<"error: "
			<<message
			<<" at "<<(report->filename ? report->filename : "<no filename>")
			<<":"<<report->lineno<<std::endl;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_write(std::ostream &out, JSContext *cx, uintN argc, jsval *vp)
	{
		jsval *argv = JS_ARGV(cx, vp);
		for (uintN  i = 0; i < argc; i++)
		{
			if(JSVAL_IS_STRING(argv[i]))
			{
				JSString *str = JSVAL_TO_STRING(argv[i]);
				size_t len = JS_GetStringLength(str);

				if(len)
				{
					jschar *chars = JS_GetStringChars(str);
					size_t dstLen = len*2;
					std::vector<char> dst(dstLen);

					(JSExceptionReporter)JS_EncodeCharacters(cx, chars, len, &dst[0], &dstLen);
					out.write(&dst[0], dstLen);
				}
			}
			else
			{
				char *s;
				if(!JS_ConvertArguments(cx, 1, argv+i, "s", &s))
				{
					if(JS_IsExceptionPending(cx))
					{
						JS_ReportPendingException(cx);
						JS_ClearPendingException(cx);
					}
					return JS_FALSE;
				}
				out.rdbuf()->sputn(s, strlen(s));
			}
		}

		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_TRUE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_write_out(JSContext *cx, uintN argc, jsval *vp)
	{
		return jsf_write(*ecx()->_out, cx, argc, vp);
	}


	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_write_err(JSContext *cx, uintN argc, jsval *vp)
	{
		switch(ecx()->_scripter->_eom)
		{
		case eomStderrOnly:
			jsf_write(*ecx()->_err, cx, argc, vp);
			break;
		case eomStderrStdout:
			{
				std::ostream &err = *ecx()->_err;
				jsf_write(err, cx, argc, vp);

				std::ostream &out = *ecx()->_out;
				if(err == std::cerr && out != std::cout)
				{
					jsf_write(out, cx, argc, vp);
				}
			}
			break;
		default:
			assert(!"unknown eom");
			jsf_write(*ecx()->_err, cx, argc, vp);
			break;
		}
		return JS_TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_write_err_ln(JSContext *cx, uintN argc, jsval *vp)
	{
		jsf_write_err(cx, argc, vp);
		*ecx()->_err << std::endl;
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_exec(JSContext *cx, uintN argc, jsval *vp)
	{
		if(1 > argc)
		{
			JS_ReportError(cx, "[exec must be called with 1 argument]");
			return JS_FALSE;
		}

		jsval *argv = JS_ARGV(cx, vp);

		//вы€вить объект this
		JSObject *objThis = NULL;
		if(JSVAL_IS_OBJECT(argv[0]))
		{
			objThis = JSVAL_TO_OBJECT(argv[0]);
			argv++;
			argc--;
		}

		//вы€вить им€ файла
		if(1 > argc)
		{
			JS_ReportError(cx, "[exec must be called with file name]");
			return JS_FALSE;
		}

		char *fname;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &fname))return false;

		std::string curFile;
		evalCurPosition(&curFile);
		std::string fullFName = ecx()->_scripter->resolveLibName(fname, curFile.c_str());
		if(fullFName.empty())
		{
			JS_ReportError(cx, "[exec file name %s not found]", fname);
			return JS_FALSE;
		}
		argv++;
		argc--;


		jsval res = ecx()->_scripter->f_execFile(fullFName, objThis, argc, NULL, argv, NULL, NULL);

		JS_SET_RVAL(ecx()->_jsCx, vp, res);
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Scripter::evalCurPosition(std::string *file, int *line)
	{
		bool res = false;

		JSErrorReporter oldReporter = JS_SetErrorReporter(ecx()->_jsCx, NULL);
		JS_ReportError(ecx()->_jsCx, "");
		JS_SetErrorReporter(ecx()->_jsCx, oldReporter);

		jsval exc;
		JS_GetPendingException(ecx()->_jsCx, &exc);

		JSErrorReport *repError = JS_ErrorFromException(ecx()->_jsCx, exc);
		if(repError)
		{
			if(file)
			{
				if(repError->filename)
				{
					*file = repError->filename;
				}
				else
				{
					file->clear();
				}
			}
			if(line)
			{
				*line = repError->lineno;
			}

			res = true;
		}

		JS_ClearPendingException(ecx()->_jsCx);
		return res;
	}


	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_setAttrs(JSContext *cx, uintN argc, jsval *vp, uintN add, uintN del)
	{
		if(2 != argc)
		{
			JS_ReportError(cx, "[setAttrs must be called with 2 arguments]");
			return JS_FALSE;
		}

		jsval *argv = JS_ARGV(cx, vp);
		if(!JSVAL_IS_OBJECT(argv[0]) || JSVAL_IS_NULL(argv[0]))
		{
			JS_ReportError(cx, "[setAttrs 1st argument must be an object]");
			return JS_FALSE;
		}
		JSObject *obj = JSVAL_TO_OBJECT(argv[0]);

		if(!JSVAL_IS_STRING(argv[1]))
		{
			JS_ReportError(cx, "[setAttrs 2nd argument must be an string]");
			return JS_FALSE;
		}

		char *propName;
		if(!JS_ConvertArguments(cx, 1, argv+1, "s", &propName))return false;

		JSBool found;
		uintN attrs;
		if(!JS_GetPropertyAttributes(cx, obj, propName, &attrs, &found))return false;
		if(!found)
		{
			JS_ReportError(cx, (std::string("[setAttrs property ")+propName+" not found]").c_str());
			return JS_FALSE;
		}
		attrs |= add;
		attrs &= uintN(-1) ^ del;
		(JSExceptionReporter)JS_SetPropertyAttributes(cx, obj, propName, attrs, &found);

		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_ro(JSContext *cx, uintN argc, jsval *vp)
	{
		return jsf_setAttrs(cx, argc, vp, JSPROP_READONLY|JSPROP_PERMANENT, 0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_rw(JSContext *cx, uintN argc, jsval *vp)
	{
		return jsf_setAttrs(cx, argc, vp, 0, JSPROP_READONLY|JSPROP_PERMANENT);
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_se(JSContext *cx, uintN argc, jsval *vp)
	{
		return jsf_setAttrs(cx, argc, vp, JSPROP_ENUMERATE, 0);
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_he(JSContext *cx, uintN argc, jsval *vp)
	{
		return jsf_setAttrs(cx, argc, vp, 0, JSPROP_ENUMERATE);
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_absFile(JSContext *cx, uintN argc, jsval *vp)
	{
		if(1 <= argc)
		{
			jsval *argv = JS_ARGV(cx, vp);
			char *s;
			if(!JS_ConvertArguments(cx, 1, argv, "s", &s))return false;

			std::string curFile; int curLine;
			Scripter::evalCurPosition(&curFile, &curLine);
			std::string target = ecx()->_scripter->resolveLibName(s, curFile.c_str());

			if(target.empty())
			{
				JS_ReportError(cx, "[absFile failed to resolve absolute name for %s]", s);
				return JS_FALSE;
			}
			JS_SET_RVAL(cx, vp, STRING_TO_JSVAL(JS_NewStringCopyN(cx, target.c_str(), target.size())));
			return JS_TRUE;
		}

		std::string curFile; int curLine;
		Scripter::evalCurPosition(&curFile, &curLine);
		JS_SET_RVAL(cx, vp, STRING_TO_JSVAL(JS_NewStringCopyN(cx, curFile.c_str(), curFile.size())));
		return JS_TRUE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	JSBool Scripter::jsf_absDir(JSContext *cx, uintN argc, jsval *vp)
	{
		const char *s;
		if(1 <= argc)
		{

			jsval *argv = JS_ARGV(cx, vp);
			if(!JS_ConvertArguments(cx, 1, argv, "s", &s))return false;
		}
		else
		{
			s = ".";
		}

		std::string curFile; int curLine;
		Scripter::evalCurPosition(&curFile, &curLine);
		std::string target = ecx()->_scripter->resolveLibName(s, curFile.c_str(), true);

		if(target.empty())
		{
			JS_ReportError(cx, "[absDir failed to resolve absolute name for %s]", s);
			return JS_FALSE;
		}
		JS_SET_RVAL(cx, vp, STRING_TO_JSVAL(JS_NewStringCopyN(cx, target.c_str(), target.size())));
		return JS_TRUE;
	}



#define JS_FN_FLAGS (JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_SHARED)
	static JSFunctionSpec shell_functions[] = {
		JS_FN("print",			Scripter::jsf_write_out,		1, JS_FN_FLAGS),	//print(str)
		JS_FN("warn_noln",		Scripter::jsf_write_err,		1, JS_FN_FLAGS),	//warn(str)
		JS_FN("warn",			Scripter::jsf_write_err_ln,		1, JS_FN_FLAGS),	//warn(str)
		JS_FN("exec",			Scripter::jsf_exec,				1, JS_FN_FLAGS),	//warn(str)
		JS_FN("ro",				Scripter::jsf_ro,				2, JS_FN_FLAGS),	//warn(str)
		JS_FN("rw",				Scripter::jsf_rw,				2, JS_FN_FLAGS),	//warn(str)
		JS_FN("se",				Scripter::jsf_se,				2, JS_FN_FLAGS),	//warn(str)
		JS_FN("he",				Scripter::jsf_he,				2, JS_FN_FLAGS),	//warn(str)
		JS_FN("absFile",		Scripter::jsf_absFile,			1, JS_FN_FLAGS),	//warn(str)
		JS_FN("absDir",			Scripter::jsf_absDir,			1, JS_FN_FLAGS),	//warn(str)
		JS_FS_END
	};

	//////////////////////////////////////////////////////////////////////////
	Scripter::Scripter(ErrorsOutMode eom)
		: _rt(NULL)
		, _cxMain(NULL)
		, _global(NULL)
		, _eom(eom)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Scripter::~Scripter()
	{
		stop();
	}


	//////////////////////////////////////////////////////////////////////////
	JSObject *Scripter::createGlobalObject(JSContext *cx)
	{
		//JS_BeginRequest(cx);
		JSObject *global = JS_NewObject(cx, &global_class, NULL, NULL);
		if (global == NULL)
		{
			//JS_EndRequest(cx);
			throw 220;
		}

		(JSExceptionReporter)JS_DefineProperty(
			cx, 
			global, 
			"global", 
			OBJECT_TO_JSVAL(global),
			NULL, NULL, 
			JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT);

		if (!JS_DefineFunctions(cx, global, shell_functions))
		{
			//JS_EndRequest(cx);
			throw 220;
		}

		/* Populate the global object with the standard globals,
		like Object and Array. */
		if (!JS_InitStandardClasses(cx, global))
		{
			//JS_EndRequest(cx);
			throw 220;
		}

		if(cx != _cxMain)
		{
			JS_SetPrototype(cx, global, JS_GetGlobalObject(_cxMain));
		}

		//JS_EndRequest(cx);

		return global;
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::start()
	{
		stop();

		JS_SetCStringsAreUTF8();
		JSBool b = JS_CStringsAreUTF8();

		/* Create a JS runtime. */
		_rt = JS_NewRuntime(32L * 1024L * 1024L);
		if (_rt == NULL)
		{
			throw 220;
		}

		JS_SetGCParameter(_rt, JSGC_MAX_BYTES, 0xffffffff);

		/* Create a context. */
		_cxMain = JS_NewContext(_rt, 8192);
		if (_cxMain == NULL)
		{
			stop();
			throw 220;
		}

		JS_SetOptions(_cxMain, 
			JSOPTION_STRICT|
			JSOPTION_VAROBJFIX|
			JSOPTION_ATLINE|
			JSOPTION_XML|
			JSOPTION_JIT|
			0);

		JS_SetVersion(_cxMain, JSVERSION_LATEST);
		JS_SetErrorReporter(_cxMain, Scripter::jsf_report_err);
		//jsuword 

		//JS_SetThreadStackLimit(_cxMain, )

		_global = createGlobalObject(_cxMain);
		JS_SetGlobalObject(_cxMain, _global);

		ecx_rw()->_jsCx = _cxMain;

		_preprocessor.setup("<%", "<%=", "%>");
		//g_instances[_cxMain] = this;

		JsObject::initClass();

	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::stop()
	{
		if(_cxMain)
		{
			//JS_BeginRequest(_cxMain);
		}


		//////////////////////////////////////////////////////////////////////////
		{
			TSMap::iterator iter = _scripts.begin();
			TSMap::iterator end = _scripts.end();
			for(; iter!=end; iter++)
			{
				JS_RemoveRoot(_cxMain, iter->second._owner.get());
				//JS_DestroyScript(_cxMain, iter->second._script);
			}
			_scripts.clear();
		}

		//////////////////////////////////////////////////////////////////////////
		{
			TFMap::iterator iter = _functions.begin();
			TFMap::iterator end = _functions.end();
			for(; iter!=end; iter++)
			{
				JS_RemoveRoot(_cxMain, iter->second._owner.get());
				//JS_DestroyFunction (_cxMain, iter->second._script);
			}
			_functions.clear();
		}
		if(_cxMain)
		{
			//JS_EndRequest(_cxMain);
		}


		if(_global)
		{
			JSIdArray *ids = JS_Enumerate(_cxMain, _global);
			if(ids)
			{
				for(jsint i(0); i<ids->length; i++)
				{
					jsval jsv;
					JS_IdToValue(_cxMain, ids->vector[i], &jsv);
					char *s;
					JS_ConvertArguments(_cxMain, 1, &jsv, "s", &s);

					JSBool found;
					(JSExceptionReporter)JS_SetPropertyAttributes(_cxMain, _global, s, JSPROP_ENUMERATE, &found);

					(JSExceptionReporter)JS_DeletePropertyById(_cxMain, _global, ids->vector[i]);
				}
				JS_DestroyIdArray(_cxMain, ids);
			}

			JS_SetGlobalObject(_cxMain, NULL);
			_global = NULL;
		}

		if(_cxMain)
		{
			//g_instances.erase(_cxMain);
			JS_GC(_cxMain);
			JS_DestroyContext(_cxMain);
			_cxMain = NULL;
		}
		ecx_rw()->_jsCx = NULL;

		if(_rt)
		{
			JS_DestroyRuntime(_rt);
			JS_ShutDown();
			_rt = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::requestStart(Request *r)
	{
		ecx_rw()->_request = r;

		if(r)
		{
			ecx_rw()->_out = &ecx()->_request->out;
			ecx_rw()->_err = &ecx()->_request->err;
		}
		//JS_BeginRequest(ecx()->_jsCx);

#define MAXJSSTACKSIZE 500000
		//setStackLimit(MAXJSSTACKSIZE);
		//JS_SetNativeStackQuota(ecx()->_jsCx, MAXJSSTACKSIZE);

	}

#ifdef DEBUG
	typedef std::map<std::string, int> TMNamedRootsCounters;
	TMNamedRootsCounters g_namedRootsCounters;
	void dump_nr(const char *name, void *rp, void *data)
	{
		(*(int *)data)++;
		//std::cout<<name<<std::endl;
		g_namedRootsCounters[name]++;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	bool requestStarted()
	{
		//JS_
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::requestStop()
	{
		//JS_EndRequest(ecx()->_jsCx);
		ecx_rw()->_out = &std::cout;
		ecx_rw()->_err = &std::cerr;

#if defined(DEBUG) && 0
		int k=0;
		g_namedRootsCounters.clear();
		JS_DumpNamedRoots(_rt, &dump_nr, &k);

		BOOST_FOREACH(const TMNamedRootsCounters::value_type &el, g_namedRootsCounters)
		{
			if(el.second>10)
			{
				std::cout<<el.second<<"\t"<<el.first<<std::endl;
			}
		}
		std::cout<<"total "<<g_namedRootsCounters.size()<<std::endl;
#endif
		ecx_rw()->_request = NULL;

		JS_MaybeGC(ecx()->_jsCx);
		//JS_GC(ecx()->_jsCx);

	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::requestGc()
	{
		//JS_MaybeGC(ecx()->_jsCx);
		JS_GC(ecx()->_jsCx);
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::idle()
	{
		assert(0);
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::setStackLimit(jsuword size)
	{
		jsuword limitAddr;
		if(size)
		{
			limitAddr = (jsuword)&limitAddr;
#if JS_STACK_GROWTH_DIRECTION > 0
			limitAddr = limitAddr + size;
#else
			limitAddr = limitAddr - size;
#endif
		}

		JS_SetThreadStackLimit(ecx()->_jsCx, limitAddr);
	}


	//////////////////////////////////////////////////////////////////////////
	TScriptID Scripter::s_compile(const std::string &source, const std::string &name)
	{
		TUtf16data preprocessed;
		if(!_preprocessor.process(source, name, preprocessed))
		{
			JS_ReportError(ecx()->_jsCx, "Scripter.compile bad source in %s", name.data());
			JS_ReportPendingException(ecx()->_jsCx);
			JS_ClearPendingException(ecx()->_jsCx);
			return NULL;
		}

		JSScript *s = JS_CompileUCScript(
			ecx()->_jsCx, 
			JS_GetGlobalObject(ecx()->_jsCx), 
			(const jschar *)(preprocessed.size()?&preprocessed[0]:NULL),
			preprocessed.size(),
			name.data(),
			1);
		TScriptID id = (TScriptID)s;
		if(id)
		{
			CompiledScript cs;
			cs._name = name;
			cs._script = s;

			cs._owner.reset(new JSObject *);
			*cs._owner = JS_NewScriptObject(ecx()->_jsCx, s);
			_scripts[id] = cs;
			JS_AddNamedRoot(ecx()->_jsCx, cs._owner.get(), _scripts[id]._name.data());
		}
		else
		{
			if(JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportPendingException(ecx()->_jsCx);
				JS_ClearPendingException(ecx()->_jsCx);
			}
		}
		return id;
	}

	//////////////////////////////////////////////////////////////////////////
	TScriptID Scripter::s_compileFile(const std::string &fname)
	{
		std::ifstream in(fname.c_str(), std::ios::binary);
		if(in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			in.seekg(0, std::ios::beg);

			std::string source;
			source.resize(size);
			in.read(const_cast<char *>(source.data()), size);
			in.close();

			return s_compile(source, fname);
		}
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::s_exec(TScriptID id, std::ostream *out, std::ostream *err)
	{
		TSMap::iterator iter = _scripts.find(id);

		if(_scripts.end() == iter)
		{
			throw 220;
			return;
		}

		ScopedStateStreams sss(out, err, iter->second._name.c_str());

		JSBool ok;
		jsval rval;
		ok = JS_ExecuteScript(ecx()->_jsCx, JS_GetGlobalObject(ecx()->_jsCx), iter->second._script, &rval);

		if(!ok && JS_IsExceptionPending(ecx()->_jsCx))
		{
			JS_ReportPendingException(ecx()->_jsCx);
			JS_ClearPendingException(ecx()->_jsCx);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::s_drop(TScriptID id)
	{
		TSMap::iterator iter = _scripts.find(id);

		if(_scripts.end() != iter)
		{
			JS_RemoveRoot(ecx()->_jsCx, iter->second._owner.get());
			//JS_DestroyScript(ecx()->_jsCx, iter->second._script);
			_scripts.erase(iter);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::s_exec(const std::string &source, const std::string &name, std::ostream *out, std::ostream *err)
	{
		ScopedStateStreams sss(out, err, name.c_str());
		TScriptID s = s_compile(source, name);
		if(s)
		{
			s_exec(s);
			s_drop(s);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::s_execFile(const std::string &fname, std::ostream *out, std::ostream *err)
	{
		ScopedStateStreams sss(out, err, fname.c_str());
		TScriptID s = s_compileFile(fname);
		if(s)
		{
			s_exec(s);
			s_drop(s);
		}
	}





















	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	TFunctionID Scripter::f_compile(const std::string &source, const std::string &name, unsigned argc, const char **argn)
	{
		TUtf16data preprocessed;
		if(!_preprocessor.process(source, name, preprocessed))
		{
			JS_ReportError(ecx()->_jsCx, "Scripter.compile bad source in %s", name.data());
			JS_ReportPendingException(ecx()->_jsCx);
			JS_ClearPendingException(ecx()->_jsCx);
			return NULL;
		}

		JSFunction *f = 
			JS_CompileUCFunction(
			ecx()->_jsCx,	// JSContext *cx, 
			NULL,			// JSObject *obj, 
			NULL,			// const char *name,
			argc,			// uintN nargs, 
			argn,			// const char **argnames,
			(const jschar *)(preprocessed.size()?&preprocessed[0]:NULL),	// const char *bytes, 
			preprocessed.size(),	// size_t length,
			name.data(),	// const char *filename, 
			1				// uintN lineno
			);


		TFunctionID id = (TFunctionID)f;
		if(id)
		{
			CompiledFunction cf;
			cf._name = name;
			cf._function = f;

			
			cf._owner.reset(new JSObject *);
			*cf._owner = JS_GetFunctionObject(f);
			_functions[id] = cf;
			JS_AddNamedRoot(ecx()->_jsCx, cf._owner.get(), _functions[id]._name.data());
		}
		else
		{
			if(JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportPendingException(ecx()->_jsCx);
				JS_ClearPendingException(ecx()->_jsCx);
			}
		}
		return id;
	}

	//////////////////////////////////////////////////////////////////////////
	TFunctionID Scripter::f_recompileInternal(
		JSFunction *internal, const std::string &name, 
		unsigned argc, const char **argn)
	{
		JSString *bodyText = JS_DecompileFunctionBody(ecx()->_jsCx, internal, 4);
		char *chtmp = JS_EncodeString(ecx()->_jsCx, bodyText);

		std::string src = "<%";
		src += chtmp;
		src += "%>";

		JS_free(ecx()->_jsCx, chtmp);
		return f_compile(src, name, argc, argn);
	}


	//////////////////////////////////////////////////////////////////////////
	TFunctionID Scripter::f_compileFile(const std::string &fname, unsigned argc, const char **argn)
	{
		std::ifstream in(fname.c_str(), std::ios::binary);
		if(in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			in.seekg(0, std::ios::beg);

			std::string source;
			source.resize(size);
			in.read(const_cast<char *>(source.data()), size);
			in.close();

			return f_compile(source, fname, argc, argn);
		}
		else
		{
			(*ecx()->_err)<<"Scripter::f_compileFile: bad file "<<fname<<std::endl;
		}
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	jsval Scripter::f_exec(TFunctionID id, JSObject*this_, unsigned argc, jsval *argv, std::ostream *out, std::ostream *err)
	{
		TFMap::iterator iter = _functions.find(id);

		if(_functions.end() == iter)
		{
			throw 220;
			return JSVAL_NULL;
		}


		ScopedStateStreams sss(out, err, iter->second._name.c_str());
		jsval rval = JSVAL_NULL;

		JSBool ok = JS_CallFunction(ecx()->_jsCx, this_, iter->second._function, argc, argv, &rval);

		if(!ok && JS_IsExceptionPending(ecx()->_jsCx))
		{
			JS_ReportPendingException(ecx()->_jsCx);
			JS_ClearPendingException(ecx()->_jsCx);
		}

		return rval;
	}

	//////////////////////////////////////////////////////////////////////////
	void Scripter::f_drop(TFunctionID id)
	{
		TFMap::iterator iter = _functions.find(id);

		if(_functions.end() != iter)
		{
			JS_RemoveRoot(ecx()->_jsCx, iter->second._owner.get());
			//JS_DestroyFunction(ecx()->_jsCx, iter->second._script);
			_functions.erase(iter);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	jsval Scripter::f_exec(const std::string &source, const std::string &name, JSObject *this_, unsigned argc, const char **argn, jsval *argv, std::ostream *out, std::ostream *err)
	{
		ScopedStateStreams sss(out, err, name.c_str());
		TFunctionID f = f_compile(source, name, argc, argn);
		if(f)
		{
			jsval res = f_exec(f, this_, argc, argv);
			f_drop(f);
			return res;
		}

		return JSVAL_NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	jsval Scripter::f_execFile(const std::string &fname, JSObject *this_, unsigned argc, const char **argn, jsval *argv, std::ostream *out, std::ostream *err)
	{
		ScopedStateStreams sss(out, err, fname.c_str());
		TFunctionID f = f_compileFile(fname, argn?argc:0, argn);
		if(f)
		{
			jsval res = f_exec(f, this_, argc, argv);
			f_drop(f);
			return res;
		}
		return JSVAL_VOID;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Scripter::addLibDir(const char *dir)
	{
		using namespace boost::filesystem;
		path d(dir);

		if(d.has_root_directory() && is_directory(d))
		{
			_libDirs.push_back(d.normalize());
			return true;
		}
		
		d = (boost::filesystem::current_path() / d).normalize();
		
		if(d.has_root_directory() && is_directory(d))
		{
			_libDirs.push_back(d);
			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	std::string Scripter::resolveLibName(const char *fname, const char *source, bool isDir)
	{
		//std::cout<<"RESOLVE '"<<(fname?fname:"NULL")<<"' in '"<<(source?source:"NULL")<<"'"<<std::endl;
		using namespace boost::filesystem;

		path fn(fname);
		if(fn.has_root_directory())
		{
			if(isDir?is_directory(fn):is_regular_file(fn))
			{
				return fn.normalize().string();
			}
			return "";
		}

		if(source && source[0])
		{
			std::string resolvedSource;
			resolvedSource = resolveLibName(source, NULL, true);
			//std::cout<<"dir: "<<resolvedSource.data()<<std::endl;
			if(resolvedSource.empty())
			{
				path sr(source);
				if(sr.has_filename() && sr.has_parent_path())
				{
					sr = sr.parent_path();
					resolvedSource = resolveLibName(sr.string().c_str(), NULL, true);
				}
			}


			if(!resolvedSource.empty())
			{
				path res = path(resolvedSource)/fn;
				if(isDir?is_directory(res):is_regular_file(res))
				{
					return res.normalize().string();
				}
			}
		}

		path res;
		for(size_t i(0); i<_libDirs.size(); i++)
		{
			res = _libDirs[i]/fn;
			if(isDir?is_directory(res):is_regular_file(res))
			{
				return res.normalize().string();
			}
		}

		res = boost::filesystem::complete(fn);
		if(isDir?is_directory(res):is_regular_file(res))
		{
			return res.normalize().string();
		}

		return "";
	}

	//////////////////////////////////////////////////////////////////////////
	bool Scripter::jsDefineInGlobal(const char *name, JsObjectPtr p)
	{
		if(!JS_DeleteProperty(ecx()->_jsCx, JS_GetGlobalObject(ecx()->_jsCx), name))return false;
		if(p)
		{
			jsval th = p->thisJsval();
			JSBool ok = JS_SetProperty(ecx()->_jsCx, JS_GetGlobalObject(ecx()->_jsCx), name, &th);
			return ok?true:false;
		}

		return true;
	}

}
