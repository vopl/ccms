#ifndef _ccms_crontab_hpp_
#define _ccms_crontab_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class Crontab
		: public JsObject
	{
	public:
		struct Task
		{
			JSObject							*_obj;
			jsval								_meth;
			boost::posix_time::time_duration	_period;
			boost::posix_time::ptime			_nextExec;

			Task()
				: _obj(NULL)
				, _meth(JSVAL_VOID)
			{

			}
		};

		typedef int32 TaskId;

	public:
		Crontab();
		~Crontab();

		TaskId add(const Task &task);
		bool del(TaskId id);

		void tick();

		bool call_add(uintN argc, jsval *argv, jsval *rval);
		bool call_del(uintN argc, jsval *argv, jsval *rval);

	private:
		TaskId _taskIdGen;

		typedef std::map<TaskId, Task> TMTasks;
		TMTasks			_tasks;

		typedef std::multimap<boost::posix_time::ptime, Task *> TMMTimeTask;
		TMMTimeTask		_orderedTasks;
	};

	typedef JsPtr<Crontab> CrontabPtr;
}
#endif
