#include "stdafx.h"
#include "crypto/rand.hpp"
#include <openssl/rand.h>
#include "crypto/utils.hpp"
#include "utils/utf8.h"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	Rand::Rand()
		: JsObject(true, "Rand")
	{
		(JSExceptionReporter)jsRegisterMeth("str", &Rand::call_str, 1);
		(JSExceptionReporter)jsRegisterMeth("str_", &Rand::call_str_, 1);
		(JSExceptionReporter)jsRegisterMeth("abc", &Rand::call_abc, 2);
		(JSExceptionReporter)jsRegisterMeth("int", &Rand::call_int, 2);
		(JSExceptionReporter)jsRegisterMeth("double", &Rand::call_double, 2);
		(JSExceptionReporter)jsRegisterMeth("bool", &Rand::call_bool, 0);

		init();
	}

	//////////////////////////////////////////////////////////////////////////
	Rand::~Rand()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	void Rand::init()
	{
#ifdef WIN32
		RAND_screen();
#else
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rand::call_str(uintN argc, jsval *argv, jsval *rval)
	{
		return call_strHelper(argc, argv, rval, true);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rand::call_str_(uintN argc, jsval *argv, jsval *rval)
	{
		return call_strHelper(argc, argv, rval, false);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rand::call_strHelper(uintN argc, jsval *argv, jsval *rval, bool is16)
	{
		int32 len=8;

		if(argc > 0)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "i", &len)) return false;
			if(len<1 || len>65536)
			{
				JS_ReportError(ecx()->_jsCx , "Rand.str first arg must be a number from 1 to 65536");
				return false;
			}
		}

		if(argc > 1)
		{
			JS_ReportError(ecx()->_jsCx , "Rand.str must be called with 0, or 1 arg");
			return false;
		}

		size_t bufLen;
		if(is16)
		{
			bufLen = len/2+1;
		}
		else
		{
			bufLen = len*3/4+1;
		}

		std::vector<unsigned char> buf(bufLen);
		RAND_bytes(&buf[0], bufLen);

		if(is16)
		{
			return mkstr16(&buf[0], bufLen, rval, len);
		}
		else
		{
			return mkstr64(&buf[0], bufLen, rval, len);
		}

		assert(0);
		return false;
	}

	namespace impl
	{
		//////////////////////////////////////////////////////////////////////////
		template <class Integral>
		void randIntRange(Integral &v, const Integral &from, const Integral &to)
		{
			unsigned long long llval;
			RAND_bytes((unsigned char *)&llval, sizeof(llval));
			v = (Integral)(double(llval) / ULLONG_MAX * (to-from+1) + from);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rand::call_abc(uintN argc, jsval *argv, jsval *rval)
	{
		if(!argc)
		{
			JS_ReportError(ecx()->_jsCx , "Rand.abc must be called with 1 or 2 args");
			return false;
		}


		char *abc8;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &abc8)) return false;
		size_t abc8len = strlen(abc8);

		std::vector<utf8::uint32_t> abc32;
		std::back_insert_iterator<std::vector<utf8::uint32_t> > bi32(abc32);
		try
		{
			utf8::utf8to32(abc8, abc8+abc8len, bi32);
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "Rand.abc first arg is invalid");
			return false;
		}

		int32 amount = 1;
		if(argc > 1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "i", &amount)) return false;
			if(amount < 1)
			{
				JS_ReportError(ecx()->_jsCx , "Rand.abc second arg must be greater 1");
				return false;
			}
		}

		std::vector<utf8::uint32_t> res32(amount);
		for(int i(0); i<amount; i++)
		{
			uint32 v;
			crypto::impl::randIntRange(v, (uint32)0, (uint32)abc32.size()-1);
			res32[i] = abc32[v];
		}

		std::string res8;
		std::back_insert_iterator<std::string> bi8(res8);
		try
		{
			utf8::utf32to8(res32.begin(), res32.end(), bi8);
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "Rand.abc internal error");
			return false;
		}

		JSString *res = JS_NewStringCopyN(ecx()->_jsCx, res8.data(), res8.size());
		if(!res)
		{
			return false;
		}

		*rval = STRING_TO_JSVAL(res);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rand::call_int(uintN argc, jsval *argv, jsval *rval)
	{
		int32 from = JSVAL_INT_MIN;
		int32 to = JSVAL_INT_MAX;

		if(argc > 0)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "i", &from)) return false;
			if(from < JSVAL_INT_MIN)
			{
				JS_ReportError(ecx()->_jsCx , "Rand.int first arg must be greater from %d", JSVAL_INT_MIN);
				return false;
			}
		}

		if(argc > 1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "i", &to)) return false;
			if(to > JSVAL_INT_MAX)
			{
				JS_ReportError(ecx()->_jsCx , "Rand.int second arg must be less from %d", JSVAL_INT_MAX);
				return false;
			}
			if(to <= from)
			{
				JS_ReportError(ecx()->_jsCx , "Rand.int second arg must be greater first");
				return false;
			}
		}
		if(argc > 2)
		{
			JS_ReportError(ecx()->_jsCx , "Rand.int must be called with 0, 1 or 2 args");
			return false;
		}

		int32 val;
		crypto::impl::randIntRange(val, from, to);

		*rval = INT_TO_JSVAL(val);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rand::call_double(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble from = 0;
		jsdouble to = 1;

		if(argc > 0)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &from)) return false;
		}

		if(argc > 1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "d", &to)) return false;
			if(to <= from)
			{
				JS_ReportError(ecx()->_jsCx , "Rand.double second arg must be greater first");
				return false;
			}
		}
		if(argc > 2)
		{
			JS_ReportError(ecx()->_jsCx , "Rand.double must be called with 0, 1 or 2 args");
			return false;
		}

		unsigned long long llval;
		RAND_bytes((unsigned char *)&llval, sizeof(llval));

		double val = double(llval) / ULLONG_MAX * (to-from) + from;

		return JS_NewDoubleValue(ecx()->_jsCx, val, rval)?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rand::call_bool(uintN argc, jsval *argv, jsval *rval)
	{
		unsigned char cval;
		RAND_bytes(&cval, 1);
		if(cval & 1)
		{
			*rval = JSVAL_TRUE;
		}
		else
		{
			*rval = JSVAL_FALSE;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Rand::onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		return call_double(argc, argv, rval);
	}

}}
