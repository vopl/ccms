#include "stdafx.h"
#include "scripter/profiler.hpp"

#if USE_PROFILER
#include <js/jsdbgapi.h>
#include <js/jsinterp.h>

#include "scripter/scripter.hpp"

#include "utils/ncvt.h"

namespace ccms
{

	//////////////////////////////////////////////////////////////////////////
	Profiler::Profiler()
	{
		_out.clear(std::ios::failbit);
	}

	//////////////////////////////////////////////////////////////////////////
	Profiler::~Profiler()
	{

	}

	//////////////////////////////////////////////////////////////////////////
#define BASE 100000
	void Profiler::start()
	{
		stop();

		JS_SetNewScriptHook(ecx()->_scripter->_rt, s_newScriptHook, this);
		JS_SetDestroyScriptHook(ecx()->_scripter->_rt, s_destroyScriptHook, this);
		JS_SetCallHook(ecx()->_scripter->_rt, s_interpretHook, this);
		JS_SetExecuteHook(ecx()->_scripter->_rt, s_interpretHook, this);

		_out.close();
		_out.clear();
		_out.open("tmon.out");
		std::cerr<<"tmon.out"<<std::endl;
		if(!_out)
		{
		    std::cerr<<"bad tmon.out"<<std::endl;
		}
		_out<<"#fOrTyTwO\n";
		_out<<"$hz="<<CLOCKS_PER_SEC<<";\n";
		_out<<"$XS_VERSION='DProf 19970606';\n";
		_out<<"# All values are given in HZ\n";
		_out<<"$rrun_utime="<<CLOCKS_PER_SEC<<"; $rrun_stime=0; $rrun_rtime=0\n";
		_out<<"PART2\n";
		_out.flush();

#ifdef WIN32
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		_liStart = li.QuadPart;
		QueryPerformanceFrequency(&li);
		_liFrequency = li.QuadPart;
#else
		_liFrequency = 1000000000;
		
		struct timespec sts;
		clock_gettime(CLOCK_REALTIME, &sts);
		_liStart = sts.tv_sec*_liFrequency + sts.tv_nsec;
		
#endif

	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::stop()
	{
		JS_SetNewScriptHook(ecx()->_scripter->_rt, NULL, this);
		JS_SetDestroyScriptHook(ecx()->_scripter->_rt, NULL, this);
		JS_SetCallHook(ecx()->_scripter->_rt, NULL, this);
		JS_SetExecuteHook(ecx()->_scripter->_rt, NULL, this);
		_out.close();
		_out.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	long long Profiler::curTime()
	{
#ifdef WIN32
		LARGE_INTEGER    li;
		QueryPerformanceCounter(&li);
		return (li.QuadPart - _liStart)*BASE/_liFrequency;
#else
		struct timespec sts;
		clock_gettime(CLOCK_REALTIME, &sts);
		unsigned long long li = sts.tv_sec*_liFrequency + sts.tv_nsec;

		return (li - _liStart)*BASE/_liFrequency;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::mark(JSContext *cx, const char *name, bool before)
	{
		if(_out)
		{
			if(cx)
			{
				JSStackFrame *fp = NULL;
				JS_FrameIterator(cx, &fp);
				JSScript* script = fp->script;

				while(fp && !script)
				{
					fp = fp->down;
					script = fp->script;
				}
				if(script)
				{
					_out<<
						(before?"+":"-")<<" "<<
						curTime()<<" 0 0 "<<
						script->filename<<":"<<script->lineno<<
						"("<<name<<")"<<std::endl;
				}
			}
			else
			{
				_out<<
					(before?"+":"-")<<" "<<
					curTime()<<" 0 0 "<<
					name<<std::endl;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::s_newScriptHook(JSContext  *cx,
		const char *filename,  /* URL of script */
		uintN      lineno,     /* first line */
		JSScript   *script,
		JSFunction *fun,
		void       *callerdata)
	{
		Profiler* self = (Profiler*) callerdata;

// 		self->mark(script->filename, script->lineno, JS_GetFunctionName(fun), true);
// 		self->mark(script->filename, script->lineno, JS_GetFunctionName(fun), false);
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::s_destroyScriptHook(JSContext *cx,
		JSScript  *script,
		void      *callerdata)
	{
		Profiler* self = (Profiler*) callerdata;
// 		self->mark(script->filename, script->lineno, JS_GetFunctionName(fun), true);
// 		self->mark(script->filename, script->lineno, JS_GetFunctionName(fun), false);
	}

	//////////////////////////////////////////////////////////////////////////
	void * Profiler::s_interpretHook(
		JSContext *cx, 
		JSStackFrame *fp, 
		JSBool before,
		JSBool *ok, 
		void *closure)
	{
		Profiler* self = (Profiler*) closure;
		self->mark(cx, fp->fun?JS_GetFunctionName(fp->fun):"unknown", before?true:false);
		return closure;
	}

	Profiler g_profiler;
}
#endif