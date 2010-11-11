#ifndef _ccms_exceptionReporter_hpp_
#define _ccms_exceptionReporter_hpp_


#include <js/jsapi.h>
#include "router/execContext.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class JSExceptionReporter
	{
	public:
		JSExceptionReporter(bool b)
		{
			check(b);
		}
		JSExceptionReporter(JSBool b)
		{
			check(b);
		}

	private:
		template <class TBool>
		TBool check(TBool b)
		{
			if(!b)
			{
				if(JS_IsExceptionPending(ecx()->_jsCx))
				{
					JS_ReportPendingException(ecx()->_jsCx);
					JS_ClearPendingException(ecx()->_jsCx);
				}
			}
			return b;
		}
	};
}



#endif
