#ifndef _ccms_scripter_hpp_
#define _ccms_scripter_hpp_

#include <js/jsapi.h>
#include "router/execContext.hpp"
#include "scripter/jsObject.hpp"
#include "scripter/preprocessor.hpp"

namespace ccms
{
//	class Request;

	
	typedef struct TScriptID_stub{} * TScriptID;
	static const TScriptID nullScript = NULL;

	typedef struct TFunctonID_stub{} * TFunctionID;
	static const TFunctionID nullFunction = NULL;

	enum ErrorsOutMode
	{
		eomStderrOnly,
		eomStderrStdout,
	};

	//////////////////////////////////////////////////////////////////////////
	class Scripter
	{
	public:
		JSRuntime	*_rt;
		JSContext	*_cxMain;
		JSObject	*_global;

		ErrorsOutMode	_eom;

		//////////////////////////////////////////////////////////////////////////
		struct CompiledScript
		{
			std::string _name;
			JSScript *_script;
			boost::shared_ptr<JSObject *> _owner;
		};

		typedef std::map<TScriptID, CompiledScript> TSMap;
		TSMap _scripts;

		//////////////////////////////////////////////////////////////////////////
		struct CompiledFunction
		{
			std::string _name;
			JSFunction *_function;
			boost::shared_ptr<JSObject *> _owner;
		};

		typedef std::map<TFunctionID, CompiledFunction> TFMap;
		TFMap _functions;

		//////////////////////////////////////////////////////////////////////////
		//Request *_r;
		static void jsf_report_err(JSContext *cx, const char *message, JSErrorReport *report);
		static JSBool jsf_write(std::ostream &out, JSContext *cx, uintN argc, jsval *vp);
		static JSBool jsf_write_err(JSContext *cx, uintN argc, jsval *vp);
		static JSBool jsf_write_err_ln(JSContext *cx, uintN argc, jsval *vp);
		static JSBool jsf_write_out(JSContext *cx, uintN argc, jsval *vp);
		static JSBool jsf_exec(JSContext *cx, uintN argc, jsval *vp);

		static JSBool jsf_setAttrs(JSContext *cx, uintN argc, jsval *vp, uintN add, uintN del);

		static JSBool jsf_ro(JSContext *cx, uintN argc, jsval *vp);
		static JSBool jsf_rw(JSContext *cx, uintN argc, jsval *vp);

		static JSBool jsf_he(JSContext *cx, uintN argc, jsval *vp);
		static JSBool jsf_se(JSContext *cx, uintN argc, jsval *vp);

		static JSBool jsf_absFile(JSContext *cx, uintN argc, jsval *vp);
		static JSBool jsf_absDir(JSContext *cx, uintN argc, jsval *vp);

		Preprocessor _preprocessor;

	public:
		static bool evalCurPosition(std::string *file, int *line=NULL);
	public:
		Scripter(ErrorsOutMode eom);
		~Scripter();

		void start();
		void stop();

		JSObject *createGlobalObject(JSContext *cx);

		void requestStart(Request *r);
		void requestStop();
		void requestGc();
		void idle();

		void setStackLimit(jsuword size);

		//массивный
		TScriptID s_compile(const std::string &source, const std::string &name);
		TScriptID s_compileFile(const std::string &fname);
		void s_exec(TScriptID id, std::ostream *out=NULL, std::ostream *err=NULL);
		void s_drop(TScriptID id);

		//разовый
		void s_exec(const std::string &source, const std::string &name, std::ostream *out=NULL, std::ostream *err=NULL);
		void s_execFile(const std::string &fname, std::ostream *out=NULL, std::ostream *err=NULL);

		//массивный
		TFunctionID f_compile(
			const std::string &source, const std::string &name, 
			unsigned argc=0, const char **argn=NULL);
		TFunctionID f_recompileInternal(
			JSFunction *internal, const std::string &name, 
			unsigned argc=0, const char **argn=NULL);
		TFunctionID f_compileFile(
			const std::string &fname, 
			unsigned argc=0, const char **argn=NULL);
		jsval f_exec(
			TFunctionID id, JSObject*this_=NULL, 
			unsigned argc=0, jsval *argv=NULL, 
			std::ostream *out=NULL, std::ostream *err=NULL);
		void f_drop(TFunctionID id);

		//разовый
		jsval f_exec(
			const std::string &source, const std::string &name, 
			JSObject*this_=NULL, 
			unsigned argc=0, const char **argn=NULL, jsval *argv=NULL, 
			std::ostream *out=NULL, std::ostream *err=NULL);
		jsval f_execFile(
			const std::string &fname, 
			JSObject*this_=NULL, 
			unsigned argc=0, const char **argn=NULL, jsval *argv=NULL, 
			std::ostream *out=NULL, std::ostream *err=NULL);

	private:
		std::vector<boost::filesystem::path> _libDirs;

	public:
		bool addLibDir(const char *dir);
		std::string resolveLibName(const char *fname, const char *source=NULL, bool isDir=false);

	public:
		bool jsDefineInGlobal(const char *name, JsObjectPtr p=JsObjectPtr());


	};

	//////////////////////////////////////////////////////////////////////////
	struct ScripterScopedRequest
	{
		ScripterScopedRequest(Request *r = NULL)
		{
			ecx()->_scripter->requestStart(r);
		}
		~ScripterScopedRequest()
		{
			ecx()->_scripter->requestStop();
		}

	};

	//////////////////////////////////////////////////////////////////////////
	struct ScripterScopedUnRequest
	{
		Request *_r;
		ScripterScopedUnRequest()
		{
			_r = ecx()->_request;
			//assert(ecx()->_scripter->requestStarted());
			ecx()->_scripter->requestStop();
		}
		~ScripterScopedUnRequest()
		{
			ecx()->_scripter->requestStart(_r);
		}

	};


	class ScopedStateStreams
	{
		std::ostream *_out;
		std::ostream *_err;
		const char *_currentName;
	public:
		ScopedStateStreams(std::ostream *out, std::ostream *err, const char *currentName = NULL);
		~ScopedStateStreams();
	};

	struct ScripterScopedStackLimit
	{
		ScripterScopedStackLimit(jsuword size)
		{
			ecx()->_scripter->setStackLimit(size);
		}
		~ScripterScopedStackLimit()
		{
			ecx()->_scripter->setStackLimit(0);
		}

	};

}
#endif
