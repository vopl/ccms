#ifndef _ccms_crypto_rand_hpp_
#define _ccms_crypto_rand_hpp_

#include "scripter/jsObject.hpp"

namespace ccms {namespace crypto
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
		bool call_str_(uintN argc, jsval *argv, jsval *rval);
		bool call_abc(uintN argc, jsval *argv, jsval *rval);
		bool call_int(uintN argc, jsval *argv, jsval *rval);
		bool call_double(uintN argc, jsval *argv, jsval *rval);
		bool call_bool(uintN argc, jsval *argv, jsval *rval);

		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	private:
		bool call_strHelper(uintN argc, jsval *argv, jsval *rval, bool is16);


	};

	typedef JsPtr<Rand> RandPtr;
}}
#endif

