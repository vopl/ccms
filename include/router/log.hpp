#ifndef _ccms_log_hpp_
#define _ccms_log_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class Log
		: public JsObject
	{
	public:
		Log();
		~Log();

		bool call_open(uintN argc, jsval *argv, jsval *rval);
		bool call_print(uintN argc, jsval *argv, jsval *rval);
		bool call_close(uintN argc, jsval *argv, jsval *rval);

	private:
		std::ofstream _out;
	};

	typedef JsPtr<Log> LogPtr;
}
#endif
