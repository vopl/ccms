#include "stdafx.h"
#include "scripter/profiler.hpp"

#if USE_PROFILER
#include <js/jsdbgapi.h>
#include <js/jsinterp.h>

#include "scripter/scripter.hpp"

#include "utils/ncvt.h"
#include "utils/crc32.hpp"

#include <iomanip>

#ifdef WIN32
#else
#	include <sys/time.h>
#	include <sys/resource.h>
#endif

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




	//////////////////////////////////////////////////////////////////////////
	void Profiler::enter(JSContext *cx, const char *name)
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
		_currentPoint = _currentPoint->enter(_timesStack, name);
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::leave()
	{
		if(!_currentPoint)
		{
			return;
		}
		_currentPoint = _currentPoint->leave(_timesStack);
	}

	//////////////////////////////////////////////////////////////////////////
	template <class T>
	void Profiler::walkPoint(
		std::deque<T> &container, 
		std::deque<std::pair<std::string, size_t> > *processedStack, 
		std::map<std::string, size_t> *processedAll, 
		const Point &p, 
		size_t level)
	{
		int prevInStackIdx = -1;
		int prevInAllIdx = -1;
		bool needStore = false;
		{
			if(processedStack)
			{
				std::deque<std::pair<std::string, size_t> >::iterator iter = processedStack->begin();
				std::deque<std::pair<std::string, size_t> >::iterator end = processedStack->end();

				for(; iter!=end; iter++)
				{
					if(iter->first == p.getName())
					{
						prevInStackIdx = iter->second;
						break;
					}
				}
			}

			if(processedAll)
			{
				typename std::map<std::string, size_t>::iterator iter = processedAll->find(p.getName());
				if(processedAll->end() != iter) prevInAllIdx = iter->second;
			}

			T cur(
				prevInStackIdx>=0?&container[prevInStackIdx]:NULL, 
				prevInAllIdx>=0?&container[prevInAllIdx]:NULL, 
				p, 
				level);

			needStore = cur.needStore();
			if(needStore)
			{
				container.push_back(cur);
			}

			if(processedAll && prevInAllIdx<0)
			{
				(*processedAll)[p.getName()] = container.size()-1;
			}

		}

		

		const Point::TMChilds &childs = p.getChilds();
		Point::TMChilds::const_iterator iter = childs.begin();
		Point::TMChilds::const_iterator end = childs.end();


		if(processedStack && needStore)
		{
			processedStack->push_back(std::make_pair(p.getName(), container.size()-1));
		}
		for(; iter!=end; iter++)
		{
			walkPoint(container, processedStack, processedAll, iter->second, level+1);
		}

		if(processedStack && needStore)
		{
			processedStack->pop_back();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Profiler::ReportLine::ReportLine(ReportLine *prevParent, ReportLine *prevSibling, const Point &p, size_t level)
		: _isDouble((prevParent || prevSibling)?true:false)
		, _level(level)
		, _ownCalls(0)
		, _childCalls(0)
	{
		_name = p.getName();

		if(prevParent)
		{
			p.accumuleTimes(_ownTimes, true, false);
			p.accumuleCalls(_ownCalls, true, false);
			
			prevParent->_ownTimes.add(_ownTimes);
			prevParent->_ownCalls += _ownCalls;
			prevParent->_childTimes.sub(_ownTimes);
			prevParent->_childCalls -= _ownCalls;
		}
		else if(prevSibling)
		{
			p.accumuleTimes(_ownTimes, true, false);
			p.accumuleCalls(_ownCalls, true, false);
			p.accumuleTimes(_childTimes, false, true);
			p.accumuleCalls(_childCalls, false, true);

			prevSibling->_ownTimes.add(_ownTimes);
			prevSibling->_ownCalls += _ownCalls;
			prevSibling->_childTimes.add(_childTimes);
			prevSibling->_childCalls += _childCalls;
		}
		else
		{
			p.accumuleTimes(_ownTimes, true, false);
			p.accumuleCalls(_ownCalls, true, false);
			p.accumuleTimes(_childTimes, false, true);
			p.accumuleCalls(_childCalls, false, true);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool Profiler::ReportLine::needStore() const
	{
		return !_isDouble;
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
	void Profiler::dumpMetricCell(std::ostream &out, T value)
	{
		out<<std::left<<std::setw(15)<<value;
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::dumpLines(
		const TDReportLines &lines, 
		std::ostream &out, 
		size_t outFormatsAmount, int *outFormats, 
		size_t amount,
		bool isTable)
	{
		Times allTime;
		size_t allCalls = 0;
		if(lines.size() > 1)
		{
			TDReportLines::const_iterator iter = lines.begin();
			TDReportLines::const_iterator end = lines.end();

			for(; iter != end; iter++)
			{
				allTime.add(iter->_ownTimes);
				allCalls += iter->_ownCalls;
			}
		}

		//out
		//std::streamsize oldPrecision = out.precision(8);
		//std::ios::fmtflags oldFlags = out.setf(std::ios::scientific);
#define T2S(t) (double(t)/1e9)
		double allTimeUser = T2S(allTime._user);
		double allTimeSystem = T2S(allTime._system);
		double allTimeReal = T2S(allTime._real);
		double allAvgTimeUser = allTimeUser;
		double allAvgTimeSystem = allTimeSystem;
		double allAvgTimeReal = allTimeReal;
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
				title += "_%";
				break;
			}

			dumpMetricCell(out, title.c_str());
		}
		out<<"\r\n";

		TDReportLines::const_iterator iter = lines.begin();
		TDReportLines::const_iterator end = lines.end();
		for(size_t counter(0); iter!=end && counter <= amount; iter++, counter++)
		{
			const ReportLine &rl = *iter;

			if(!isTable)
			{
				for(size_t i(1); i<rl._level; i++)
				{
					out<<" ";
				}
			}

			for(size_t formatIdx(0); formatIdx<outFormatsAmount; formatIdx++)
			{
				int format = outFormats[formatIdx];

				bool isPercent = (format & epmmPercent)?true:false;

				switch((EProfilerMetricKind)(format & epmkMask))
				{
				case epmkUser:
					if(isPercent)
					{
						dumpMetricCell(out, allTimeUser?(T2S(rl.getMetric(format))/allTimeUser*100):0);
					}
					else
					{
						dumpMetricCell(out, T2S(rl.getMetric(format)));
					}
					break;

				case epmkSystem:
					if(isPercent)
					{
						dumpMetricCell(out, allTimeSystem?(T2S(rl.getMetric(format))/allTimeSystem*100):0);
					}
					else
					{
						dumpMetricCell(out, T2S(rl.getMetric(format)));
					}
					break;
				default:
				case epmkReal:
					if(isPercent)
					{
						dumpMetricCell(out, allTimeReal?(T2S(rl.getMetric(format))/allTimeReal*100):0);
					}
					else
					{
						dumpMetricCell(out, T2S(rl.getMetric(format)));
					}
					break;
				case epmkAvgUser:
					if(isPercent)
					{
						dumpMetricCell(out, allAvgTimeUser?(T2S(rl.getMetric(format))/allAvgTimeUser*100):0);
					}
					else
					{
						dumpMetricCell(out, T2S(rl.getMetric(format)));
					}
					break;
				case epmkAvgSystem:
					if(isPercent)
					{
						dumpMetricCell(out, allAvgTimeSystem?(T2S(rl.getMetric(format))/allAvgTimeSystem*100):0);
					}
					else
					{
						dumpMetricCell(out, T2S(rl.getMetric(format)));
					}
					break;
				case epmkAvgReal:
					if(isPercent)
					{
						dumpMetricCell(out, allAvgTimeReal?(T2S(rl.getMetric(format))/allAvgTimeReal*100):0);
					}
					else
					{
						dumpMetricCell(out, T2S(rl.getMetric(format)));
					}
					break;
				case epmkCalls:
					if(isPercent)
					{
						dumpMetricCell(out, allCalls?(double(rl.getMetric(format))/allCalls*100):0);
					}
					else
					{
						dumpMetricCell(out, rl.getMetric(format));
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
	void Profiler::dumpTable(
		std::ostream &out,
		size_t outFormatsAmount,
		int *outFormats, 
		int orderFormat, 
		size_t amount)
	{
		//linearize
		TDReportLines lines;
		{
			std::deque<std::pair<std::string, size_t> > processedStack;
			std::map<std::string, size_t> processedAll;
			walkPoint(lines, &processedStack, &processedAll, _rootPoint);
		}

		//remove root
		lines.pop_front();

		//sort
		std::sort(lines.begin(), lines.end(), Pred4Table(orderFormat));

		dumpLines(lines, out, outFormatsAmount, outFormats, amount);
	}

	//////////////////////////////////////////////////////////////////////////
	void Profiler::dumpTree(
		std::ostream &out, 
		size_t outFormatsAmount,
		int *outFormats)
	{
		//linearize
		TDReportLines lines;
		walkPoint(lines, NULL, NULL, _rootPoint);

		//remove root
		lines.pop_front();

		dumpLines(lines, out, outFormatsAmount, outFormats, (size_t)-1, false);
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
		_real = (unsigned long long)(_counterMult * li.QuadPart);

		//structure contains two 32-bit values that combine to form a 64-bit count of 100-nanosecond time units.
		FILETIME CreationTime;
		FILETIME ExitTime;
		FILETIME KernelTime;
		FILETIME UserTime;
		GetThreadTimes(_hCurrentThread, &CreationTime, &ExitTime, &KernelTime, &UserTime);
		_user = ((unsigned long long)UserTime.dwHighDateTime<<32) | UserTime.dwLowDateTime;
		_user = _user * 100;
		_system = ((unsigned long long)KernelTime.dwHighDateTime<<32) | KernelTime.dwLowDateTime;
		_system = _system * 100;
#else
		struct timespec sts;
// 		clock_gettime(CLOCK_REALTIME, &sts);
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &sts);
		_real = (unsigned long long)sts.tv_sec*1000000000 + sts.tv_nsec;

		struct rusage ru = {};

		if(getrusage(RUSAGE_THREAD, &ru))
		//if(getrusage(RUSAGE_SELF, &ru))
		{
			//error
			_user = 0;
			_system = 0;
		}
		else
		{
			_user = (unsigned long long)ru.ru_utime.tv_sec*1000000000 + ru.ru_utime.tv_usec*1000;
			_system = (unsigned long long)ru.ru_stime.tv_sec*1000000000 + ru.ru_stime.tv_usec*1000;
		}
		
		_system = 0;
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
	double Profiler::Times::_counterMult = 1;
	HANDLE Profiler::Times::_hCurrentThread = INVALID_HANDLE_VALUE;
#endif

	//////////////////////////////////////////////////////////////////////////
	void Profiler::Times::calibrate()
	{
#ifdef WIN32
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		_counterMult = double(1e9)/li.QuadPart;

		_hCurrentThread = GetCurrentThread();
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
	Profiler::Point *Profiler::Point::leave(TSTimes &timesStack)
	{
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
	void Profiler::Point::accumuleTimes(Times &times, bool own, bool childs) const
	{
		if(own && childs)
		{
			times.add(_times);
			return;
		}

		if(own)
		{
			times.add(_times);
			TMChilds::const_iterator iter = _childs.begin();
			TMChilds::const_iterator end = _childs.end();

			for(; iter!=end; iter++)
			{
				times.sub(iter->second._times);
			}
			return;
		}

		if(childs)
		{
			TMChilds::const_iterator iter = _childs.begin();
			TMChilds::const_iterator end = _childs.end();

			for(; iter!=end; iter++)
			{
				times.add(iter->second._times);
			}
			return;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	void Profiler::Point::accumuleCalls(size_t &calls, bool own, bool childs) const
	{
		if(own)
		{
			calls += _calls;
		}

		if(childs)
		{
			TMChilds::const_iterator iter = _childs.begin();
			TMChilds::const_iterator end = _childs.end();

			for(; iter!=end; iter++)
			{
				iter->second.accumuleCalls(calls, true, true);
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
		if(before)
		{
			self->enter(cx, fp->fun?JS_GetFunctionName(fp->fun):"unknown_js_function");
		}
		else
		{
			self->leave();
		}
		return closure;
	}

	Profiler g_profiler;
}

#endif
