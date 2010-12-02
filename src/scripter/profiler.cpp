#include "stdafx.h"
#include "scripter/profiler.hpp"

#if USE_PROFILER
#include <js/jsdbgapi.h>
#include <js/jsinterp.h>

#include "scripter/scripter.hpp"

#include "utils/ncvt.h"
#include "utils/crc32.hpp"

namespace ccms
{

	//////////////////////////////////////////////////////////////////////////
	Profiler::Profiler()
	{
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

		reset();
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::stop()
	{
		JS_SetNewScriptHook(ecx()->_scripter->_rt, NULL, this);
		JS_SetDestroyScriptHook(ecx()->_scripter->_rt, NULL, this);
		JS_SetCallHook(ecx()->_scripter->_rt, NULL, this);
		JS_SetExecuteHook(ecx()->_scripter->_rt, NULL, this);

		reset();

	}
	
	//////////////////////////////////////////////////////////////////////////
	void Profiler::reset()
	{
		Times::calibrate();

		while(!_timesStack.empty()) _timesStack.pop();
		_rootPoint.reset(NULL, "root");
		_currentPoint = &_rootPoint;
	}



// 	//////////////////////////////////////////////////////////////////////////
// 	long long Profiler::curTime()
// 	{
// #ifdef WIN32
// 		LARGE_INTEGER    li;
// 		QueryPerformanceCounter(&li);
// 		return (li.QuadPart - _liStart)*BASE/_liFrequency;
// #else
// 		struct timespec sts;
// 		clock_gettime(CLOCK_REALTIME, &sts);
//		unsigned long long li = sts.tv_sec*_liFrequency + sts.tv_nsec;
//
//		return (li - _liStart)*BASE/_liFrequency;
// #endif
// 	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::mark(JSContext *cx, const char *name, bool before)
	{
		if(!_currentPoint)
		{
			return;
		}

		char buf[4096];
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
				sprintf(buf, "%s:%u(%s)", script->filename, script->lineno, name);
				name = buf;
			}
		}
		_currentPoint = before?_currentPoint->enter(_timesStack, name):_currentPoint->leave(_timesStack, name);
	}

	//////////////////////////////////////////////////////////////////////////
	template <class T>
	void Profiler::walkPoint(std::deque<T> &container, const Point &p, size_t level)
	{
		container.push_back(T(p, level));

		const Point::TMChilds &childs = p.getChilds();
		Point::TMChilds::const_iterator iter = childs.begin();
		Point::TMChilds::const_iterator end = childs.end();

		for(; iter!=end; iter++)
		{
			walkPoint(container, iter->second, level+1);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Profiler::ReportLine::ReportLine(const Point &p, size_t level)
		: _level(level)
		, _ownCalls(0)
		, _childCalls(0)
	{
		_name = p.getName();
		p.accumuleMetrics(_ownTimes, _ownCalls, true, false);
		p.accumuleMetrics(_childTimes, _childCalls, false, true);
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::ReportLine::add(const ReportLine &rl)
	{
		_ownTimes.add(rl._ownTimes);
		_ownCalls += rl._ownCalls;
		_childTimes.add(rl._childTimes);
		_childCalls += rl._childCalls;
	}

	//////////////////////////////////////////////////////////////////////////
	unsigned long long Profiler::ReportLine::getMetric(int format) const
	{
		EProfilerMetricKind kind = (EProfilerMetricKind)(format & epmkMask);
		EProfilerMetricArea area = (EProfilerMetricArea)(format & epmaMask);

		unsigned long long mo, mc;
		switch(kind)
		{
		case epmkUser:
			mo = _ownTimes._user;
			mc = _childTimes._user;
			break;
		case epmkSystem:
			mo = _ownTimes._system;
			mc = _childTimes._system;
			break;
		default:
		case epmkReal:
			mo = _ownTimes._real;
			mc = _childTimes._real;
			break;
		case epmkAvgUser:
			mo = _ownCalls?_ownTimes._user / _ownCalls:0;
			mc = _childCalls?_childTimes._user / _childCalls:0;
			break;
		case epmkAvgSystem:
			mo = _ownCalls?_ownTimes._system / _ownCalls:0;
			mc = _childCalls?_childTimes._system / _childCalls:0;
			break;
		case epmkAvgReal:
			mo = _ownCalls?_ownTimes._real / _ownCalls:0;
			mc = _childCalls?_childTimes._real / _childCalls:0;
			break;
		case epmkCalls:
			mo = _ownCalls;
			mc = _childCalls;
			break;
		}

		switch(area)
		{
		default:
		case epmaOwn:
			return mo;
		case epmaChilds:
			return mc;
		case epmaAll:
			return mo+mc;
		}
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Profiler::PredName::operator()(const Profiler::ReportLine &l, const Profiler::ReportLine &r)
	{
		if(l._name > r._name) return true;
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	Profiler::Pred4Table::Pred4Table(int orderFormat)
		: _orderFormat(orderFormat)
	{
	}

	bool Profiler::Pred4Table::operator()(const Profiler::ReportLine &l, const Profiler::ReportLine &r)
	{
		return l.getMetric(_orderFormat) > r.getMetric(_orderFormat);
	}

	//////////////////////////////////////////////////////////////////////////
	template<class T>
	void Profiler::dumpTableCell(std::ostream &out, const char *delim, T value)
	{
		std::streamsize oldWidth = out.width(15);
		out<<value<<delim;
		out.width(oldWidth);
	}


	//////////////////////////////////////////////////////////////////////////
	void Profiler::dumpTable(
		std::ostream &out, const char *delim, 
		size_t outFormatsAmount,
		int *outFormats, 
		int orderFormat, 
		size_t amount)
	{
		//linearize
		typedef std::deque<ReportLine> TDReportLines;
		TDReportLines lines;
		walkPoint(lines, _rootPoint);

		//merge equal names
		Times allTime;
		unsigned long long llAllCalls = 0;
		if(lines.size() > 1)
		{
			std::sort(lines.begin(), lines.end(), PredName());
			TDReportLines::iterator iter = ++lines.begin();

			while(iter != lines.end())
			{
				allTime.add(iter->_ownTimes);
				llAllCalls += iter->_ownCalls;
				TDReportLines::iterator prevIter = iter-1;
				if(iter->_name == prevIter->_name)
				{
					prevIter->add(*iter);
					iter = lines.erase(iter);
				}
				else
				{
					iter++;
				}
			}
		}

		//remove root
		lines.pop_front();


		
		//sort
		std::sort(lines.begin(), lines.end(), Pred4Table(orderFormat));

		//out
		//std::streamsize oldPrecision = out.precision(8);
 		//std::ios::fmtflags oldFlags = out.setf(std::ios::scientific);
#define T2S(t) (double(t)/1e9)
		double allTimeUser = T2S(allTime._user);
		double allTimeSystem = T2S(allTime._system);
		double allTimeReal = T2S(allTime._real);
		double allAvgTimeUser = llAllCalls?allTimeUser/llAllCalls:0;
		double allAvgTimeSystem = llAllCalls?allTimeSystem/llAllCalls:0;
		double allAvgTimeReal = llAllCalls?allTimeReal/llAllCalls:0;
		out<<"u: "<<allTimeUser<<"\t s:"<<allTimeSystem<<"\t r:"<<allTimeReal<<"\r\n";

		for(size_t formatIdx(0); formatIdx<outFormatsAmount; formatIdx++)
		{
			int format = outFormats[formatIdx];

			std::string title;

			switch((EProfilerMetricArea)(format & epmaMask))
			{
			default:
			case epmaOwn:
				title += "own";
				break;
			case epmaChilds:
				title += "childs";
				break;
			case epmaAll:
				title += "all";
				break;
			}

			title += "_";

			switch((EProfilerMetricKind)(format & epmkMask))
			{
			case epmkUser:
				title += "usr";
				break;
			case epmkSystem:
				title += "sys";
				break;
			default:
			case epmkReal:
				title += "real";
				break;
			case epmkAvgUser:
				title += "avg_usr";
				break;
			case epmkAvgSystem:
				title += "avg_sys";
				break;
			case epmkAvgReal:
				title += "avg_real";
				break;
			case epmkCalls:
				title += "calls";
				break;
			}

			switch((EProfilerMetricMode)(format & epmmMask))
			{
			default:
			case epmmUnit:
				break;
			case epmmPercent:
				title += "%";
				break;
			}

			dumpTableCell(out, delim, title.c_str());
		}
		out<<"\r\n";

		TDReportLines::iterator iter = lines.begin();
		TDReportLines::iterator end = lines.end();
		for(size_t counter(0); iter!=end && counter <= amount; iter++, counter++)
		{
			ReportLine &rl = *iter;
			for(size_t formatIdx(0); formatIdx<outFormatsAmount; formatIdx++)
			{
				int format = outFormats[formatIdx];

				bool isPercent = (format & epmmPercent)?true:false;

				switch((EProfilerMetricKind)(format & epmkMask))
				{
				case epmkUser:
					if(isPercent)
					{
						dumpTableCell(out, delim, allTimeUser?(T2S(rl.getMetric(format))/allTimeUser*100):0);
					}
					else
					{
						dumpTableCell(out, delim, T2S(rl.getMetric(format)));
					}
					break;

				case epmkSystem:
					if(isPercent)
					{
						dumpTableCell(out, delim, allTimeSystem?(T2S(rl.getMetric(format))/allTimeSystem*100):0);
					}
					else
					{
						dumpTableCell(out, delim, T2S(rl.getMetric(format)));
					}
					break;
				default:
				case epmkReal:
					if(isPercent)
					{
						dumpTableCell(out, delim, allTimeReal?(T2S(rl.getMetric(format))/allTimeReal*100):0);
					}
					else
					{
						dumpTableCell(out, delim, T2S(rl.getMetric(format)));
					}
					break;
				case epmkAvgUser:
					if(isPercent)
					{
						dumpTableCell(out, delim, allAvgTimeUser?(T2S(rl.getMetric(format))/allAvgTimeUser*100):0);
					}
					else
					{
						dumpTableCell(out, delim, T2S(rl.getMetric(format)));
					}
					break;
				case epmkAvgSystem:
					if(isPercent)
					{
						dumpTableCell(out, delim, allAvgTimeSystem?(T2S(rl.getMetric(format))/allAvgTimeSystem*100):0);
					}
					else
					{
						dumpTableCell(out, delim, T2S(rl.getMetric(format)));
					}
					break;
				case epmkAvgReal:
					if(isPercent)
					{
						dumpTableCell(out, delim, allAvgTimeReal?(T2S(rl.getMetric(format))/allAvgTimeReal*100):0);
					}
					else
					{
						dumpTableCell(out, delim, T2S(rl.getMetric(format)));
					}
					break;
				case epmkCalls:
					if(isPercent)
					{
						dumpTableCell(out, delim, llAllCalls?(double(rl.getMetric(format))/llAllCalls*100):0);
					}
					else
					{
						dumpTableCell(out, delim, rl.getMetric(format));
					}
					break;
				}
			}


			out<<rl._name<<"\r\n";
		}
		out.flush();

 		//out.precision(oldPrecision);
 		//out.setf(oldFlags);
	}

	//////////////////////////////////////////////////////////////////////////
	Profiler::Times::Times()
		: _user(0)
		, _system(0)
		, _real(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::Times::now()
	{
#ifdef WIN32
		LARGE_INTEGER    li;
		QueryPerformanceCounter(&li);
		_real = li.QuadPart * 1000000000 / _liFrequency;

		//structure contains two 32-bit values that combine to form a 64-bit count of 100-nanosecond time units.
		FILETIME CreationTime;
		FILETIME ExitTime;
		FILETIME KernelTime;
		FILETIME UserTime;
		GetThreadTimes(GetCurrentThread(), &CreationTime, &ExitTime, &KernelTime, &UserTime);
		_user = ((unsigned long long)UserTime.dwHighDateTime<<32) | UserTime.dwLowDateTime;
		_user = _user * 100;
		_system = ((unsigned long long)KernelTime.dwHighDateTime<<32) | KernelTime.dwLowDateTime;
		_system = _system * 100;
#else
		struct timespec sts;
		clock_gettime(CLOCK_REALTIME, &sts);
		_real = (unsigned long long)sts.tv_sec*1000000000 + sts.tv_nsec;

		assert(!"_user, _system");
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::Times::sub(const Times &t)
	{
		_user -= t._user;
		_system -= t._system;
		_real -= t._real;
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::Times::add(const Times &t)
	{
		_user += t._user;
		_system += t._system;
		_real += t._real;
	}

	//////////////////////////////////////////////////////////////////////////
#ifdef WIN32
	unsigned long long Profiler::Times::_liFrequency=1;
#endif

	//////////////////////////////////////////////////////////////////////////
	void Profiler::Times::calibrate()
	{
#ifdef WIN32
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		_liFrequency = li.QuadPart;
#endif
	}



	//////////////////////////////////////////////////////////////////////////
	Profiler::Point::Point()
		: _name()
		, _calls(0)
		, _parent(NULL)
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	Profiler::Point::~Point()
	{

	}

	void Profiler::Point::reset(Point *parent, const char *name)
	{
		_name = name;
		_calls = 0;
		_times._user = 0;
		_times._system = 0;
		_times._real = 0;

		_childs.clear();
		_parent = parent;
	}

	//////////////////////////////////////////////////////////////////////////
	Profiler::Point *Profiler::Point::enter(TSTimes &timesStack, const char *name)
	{
		timesStack.push(Times());
		timesStack.top().now();

		boost::uint32_t id = Crc32(name, strlen(name));

		Point *child;
		TMChilds::iterator iter = _childs.find(id);
		if(_childs.end() == iter)
		{
			child = &_childs[id];
			child->reset(this, name);
		}
		else
		{
			child = &iter->second;
		}

		return child;
	}

	//////////////////////////////////////////////////////////////////////////
	Profiler::Point *Profiler::Point::leave(TSTimes &timesStack, const char *name)
	{
		assert(_name == name);
		assert(_parent);

		Times t;
		t.now();

		t.sub(timesStack.top());
		timesStack.pop();

		_calls++;
		_times.add(t);

		return _parent;
	}

	//////////////////////////////////////////////////////////////////////////
	const std::string &Profiler::Point::getName() const
	{
		return _name;
	}

	//////////////////////////////////////////////////////////////////////////
	const Profiler::Point::TMChilds &Profiler::Point::getChilds() const
	{
		return _childs;
	}


	//////////////////////////////////////////////////////////////////////////
	void Profiler::Point::accumuleMetrics(Times &times, size_t &calls, bool own, bool childs) const
	{
		if(own && childs)
		{
			times.add(_times);
			calls += _calls;
			TMChilds::const_iterator iter = _childs.begin();
			TMChilds::const_iterator end = _childs.end();

			for(; iter!=end; iter++)
			{
				calls += iter->second._calls;
			}
			return;
		}
		else if(own)
		{
			times.add(_times);
			calls += _calls;
			TMChilds::const_iterator iter = _childs.begin();
			TMChilds::const_iterator end = _childs.end();

			for(; iter!=end; iter++)
			{
				times.sub(iter->second._times);
			}
			return;
		}
		else if(childs)
		{
			TMChilds::const_iterator iter = _childs.begin();
			TMChilds::const_iterator end = _childs.end();

			for(; iter!=end; iter++)
			{
				iter->second.accumuleMetrics(times, calls, true, true);
			}
		}
		else
		{
			//nothing
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