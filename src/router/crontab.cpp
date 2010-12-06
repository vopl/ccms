#include "stdafx.h"
#include "router/crontab.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Crontab::Crontab()
		: JsObject(true, "Crontab")
		, _taskIdGen(1)
	{
		jsRegisterMeth("add", &Crontab::call_add, 1);
		jsRegisterMeth("del", &Crontab::call_del, 1);
	}

	//////////////////////////////////////////////////////////////////////////
	Crontab::~Crontab()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Crontab::TaskId Crontab::add(const Task &task)
	{
		TaskId id = _taskIdGen++;
		Task &t = _tasks[id] = task;
		t._nextExec = boost::posix_time::microsec_clock::local_time();
		JS_AddNamedRoot(ecx()->_jsCx, &t._meth, __FUNCTION__);
		if(t._obj)
		{
			JS_AddNamedRoot(ecx()->_jsCx, &t._obj, __FUNCTION__);
		}

		return id;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Crontab::del(TaskId id)
	{
		TMTasks::iterator iter = _tasks.find(id);
		if(_tasks.end() == iter)
		{
			return false;
		}
		Task &t = iter->second;
		JS_RemoveRoot(ecx()->_jsCx, &t._meth);
		if(t._obj)
		{
			JS_RemoveRoot(ecx()->_jsCx, &t._obj);
		}
		_tasks.erase(iter);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void Crontab::tick()
	{
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		BOOST_FOREACH(TMTasks::value_type &el, _tasks)
		{
			if(now > el.second._nextExec)
			{
				while(now > el.second._nextExec)
				{
					el.second._nextExec += el.second._period;
				}

				jsval rval;
				jsval argv[1] = {INT_TO_JSVAL(el.first)};
				JS_CallFunctionValue(ecx()->_jsCx, el.second._obj, el.second._meth, 1, argv, &rval);
			}

		}

	}

	//////////////////////////////////////////////////////////////////////////
	void Crontab::reset()
	{
		while(!_tasks.empty())
		{
			del(_tasks.begin()->first);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	bool Crontab::call_add(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Crontab.add must be called with 1 argument]");
			return false;
		}

		if(JSVAL_IS_NULL(argv[0]) || !JSVAL_IS_OBJECT(argv[0]))
		{
			JS_ReportError(ecx()->_jsCx, "[Crontab.add must be called with an conf-object]");
			return false;
		}

		JSObject *obj =JSVAL_TO_OBJECT(argv[0]);
		JSContext *cx = ecx()->_jsCx;

		Task task;

		////////////////
		if(!JS_GetProperty(cx, obj, "meth", &task._meth))return false;
		if(!JSVAL_IS_OBJECT(task._meth) || JSVAL_IS_NULL(task._meth))
		{
			JS_ReportError(ecx()->_jsCx, "[Crontab.add meth must be setted and must be an function]");
			return false;
		}

		////////////////
		jsval jsv;
		if(!JS_GetProperty(cx, obj, "obj", &jsv))return false;
		if(JSVAL_IS_OBJECT(jsv))
		{
			task._obj = JSVAL_TO_OBJECT(jsv);
		}
		else
		{
			if(!JSVAL_IS_VOID(jsv))
			{
				JS_ReportError(ecx()->_jsCx, "[Crontab.add obj must an object or undefined]");
				return false;
			}
		}


		////////////////
		if(!JS_GetProperty(cx, obj, "period", &jsv))return false;
		if(!JSVAL_IS_VOID(jsv))
		{
			jsdouble per=0;
			if(!JS_ConvertArguments(cx, 1, &jsv, "I", &per))
			{
				JS_ReportError(ecx()->_jsCx, "[Crontab.add period must an number of milliseconds]");
				return false;
			}

			if(!per)
			{
				JS_ReportError(ecx()->_jsCx, "[Crontab.add period must not ne a null]");
				return false;
			}

			task._period = boost::posix_time::millisec(boost::int64_t(per));
		}


		*rval = INT_TO_JSVAL(add(task));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Crontab::call_del(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Crontab.del must be called with 1 argument]");
			return false;
		}
		TaskId id;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "i", &id))
		{
			JS_ReportError(ecx()->_jsCx, "[Crontab.del argument must an task id from Crontab.add]");
			return false;
		}

		*rval = del(id)?JSVAL_TRUE:JSVAL_FALSE;
		return true;
	}
}
