#ifndef _ccms_profiler_hpp_
#define _ccms_profiler_hpp_


#define USE_PROFILER 0

#if USE_PROFILER

#include <js/jsapi.h>

#ifdef WIN32
#else
#	include <sys/times.h>
#endif


namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class Profiler
	{
		std::ofstream _out;
		unsigned long long    _liStart, _liFrequency;
		long long curTime();

	public:
		Profiler();
		~Profiler();

		void start();
		void stop();

		void mark(JSContext *cx, const char *name, bool before);

	private:
		static void s_newScriptHook(JSContext  *cx,
			const char *filename,  /* URL of script */
			uintN      lineno,     /* first line */
			JSScript   *script,
			JSFunction *fun,
			void       *callerdata);

		static void s_destroyScriptHook(JSContext *cx,
			JSScript  *script,
			void      *callerdata);

		static void * s_interpretHook(
			JSContext *cx, 
			JSStackFrame *fp, 
			JSBool before,
			JSBool *ok, 
			void *closure);
	};

	extern Profiler g_profiler;

	//////////////////////////////////////////////////////////////////////////
	class ProfilerScopeHelper
	{
		Profiler &_prof;
		JSContext *_cx;
		const char *_name;

	public:
		ProfilerScopeHelper(Profiler &prof, JSContext *cx, const char *name)
			: _prof(prof)
			, _cx(cx)
			, _name(name)
		{
			_prof.mark(_cx, _name, true);
		}

		~ProfilerScopeHelper()
		{
			_prof.mark(_cx, _name, false);
		}
	};

}

#endif
#endif
