#ifndef _ccms_rand_hpp_
#define _ccms_rand_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class Rand
		: public JsObject
	{
	public:
		Rand();
		~Rand();

		void init();

		bool call_str(uintN argc, jsval *argv, jsval *rval);
		bool call_int(uintN argc, jsval *argv, jsval *rval);
		bool call_double(uintN argc, jsval *argv, jsval *rval);
		bool call_bool(uintN argc, jsval *argv, jsval *rval);
	};

	typedef JsPtr<Rand> RandPtr;
}
#endif

