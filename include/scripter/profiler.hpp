#ifndef _ccms_profiler_hpp_
#define _ccms_profiler_hpp_


#define USE_PROFILER 1

#if USE_PROFILER

#include <js/jsapi.h>

#ifdef WIN32
#else
#	include <sys/times.h>
#endif


namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	enum EProfilerMetricKind
	{
		epmkUser		=0x2,
		epmkSystem		=0x4,
		epmkReal		=0x8,

		epmkAvgUser		=0x20,
		epmkAvgSystem	=0x40,
		epmkAvgReal		=0x80,

		epmkCalls		=0x100,

		epmkMask		=0xffff,
	};

	//////////////////////////////////////////////////////////////////////////
	enum EProfilerMetricArea
	{
		epmaOwn		= 0x10000,
		epmaChilds	= 0x20000,
		epmaAll		= 0x30000,
		epmaMask	= 0xf0000,
	};

	//////////////////////////////////////////////////////////////////////////
	enum EProfilerMetricMode
	{
		epmmUnit	= 0x100000,
		epmmPercent	= 0x200000,

		epmmMask	= 0xf00000,
	};

	//////////////////////////////////////////////////////////////////////////
	class Profiler
	{
	public:
		Profiler();
		~Profiler();

		void start();
		void stop();
		void reset();

		void enter(JSContext *cx, const char *name);
		void leave();

		void dumpTable(
			std::ostream &out, const char *delim, 
			size_t outFormatsAmount,
			int *outFormats, 
			int orderFormat, 
			size_t amount = (size_t)-1);
		void dumpTree(
			std::ostream &out, const char *delim, 
			int kind, int area);

		template<class T>
		void dumpTableCell(std::ostream &out, const char *delim, T value);

	private:
		//////////////////////////////////////////////////////////////////////////
		struct Times
		{
			unsigned long long	_user;
			unsigned long long	_system;
			unsigned long long	_real;

			Times();
			void now();
			void sub(const Times &t);
			void add(const Times &t);
#ifdef WIN32
			static double _counterMult;
			static HANDLE _hCurrentThread;
#endif
			static void calibrate();
		};
		typedef std::stack<Times, std::vector<Times> > TSTimes;
		TSTimes _timesStack;

		//////////////////////////////////////////////////////////////////////////
		class Point
		{
		public:
			typedef std::map<boost::uint32_t, Point> TMChilds;
		private:
			std::string		_name;
			size_t			_calls;
			Times			_times;

			TMChilds _childs;
			Point *_parent;
		public:
			Point();
			~Point();

			void reset(Point *parent, const char *name);
			Point *enter(TSTimes &timesStack, const char *name);
			Point *leave(TSTimes &timesStack);

			const std::string &getName()const;
			const TMChilds &getChilds()const;
			void accumuleTimes(Times &times, bool own, bool childs)const;
			void accumuleCalls(size_t &calls, bool own, bool childs)const;
		};
		Point _rootPoint;
		Point *_currentPoint;

	private:
		template <class T>
		void walkPoint(
			std::deque<T> &container, 
			std::deque<std::pair<std::string, size_t> > &processedStack, 
			std::map<std::string, size_t> &processedAll, 
			const Point &p, size_t level=0);

		struct ReportLine
		{
			bool _isDouble;
			size_t	_level;
			std::string _name;
			Times _ownTimes;
			size_t _ownCalls;
			Times _childTimes;
			size_t _childCalls;

			ReportLine(ReportLine *prevParent, ReportLine *prevSibling, const Point &p, size_t level);
			bool needStore() const;
			void add(const ReportLine &rl);
			unsigned long long getMetric(int format) const;
		};

		//////////////////////////////////////////////////////////////////////////
		struct PredName
		{
			bool operator()(const Profiler::ReportLine &l, const Profiler::ReportLine &r);
		};
		struct Pred4Table
		{
			int _orderFormat;
			Pred4Table(int orderFormat);
			bool operator()(const Profiler::ReportLine &l, const Profiler::ReportLine &r);
		};


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

	public:
		ProfilerScopeHelper(Profiler &prof, JSContext *cx, const char *name)
			: _prof(prof)
		{
			_prof.enter(cx, name);
		}

		~ProfilerScopeHelper()
		{
			_prof.leave();
		}
	};

}

#endif
#endif
