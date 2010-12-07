#include "stdafx.h"
#include "crypto/rand.hpp"
#include <openssl/rand.h>
#include "crypto/utils.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Rand::Rand()
		: JsObject(true, "Rand")
	{
		(JSExceptionReporter)jsRegisterMeth("str", &Rand::call_str, 1);
		(JSExceptionReporter)jsRegisterMeth("int", &Rand::call_int, 0);
		(JSExceptionReporter)jsRegisterMeth("double", &Rand::call_double, 0);
		(JSExceptionReporter)jsRegisterMeth("bool", &Rand::call_bool, 0);
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
		int32 len=8;
		int32 base=16;

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
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "i", &base)) return false;
			if(base!=16 && base!=64)
			{
				JS_ReportError(ecx()->_jsCx , "Rand.str second arg must be a number 16 or 64");
				return false;
			}
		}
		if(argc > 2)
		{
			JS_ReportError(ecx()->_jsCx , "Rand.str must be called with 0, 1 or 2 args");
			return false;
		}

		size_t bufLen;
		switch(base)
		{
		case 16:
			bufLen = len/2;
			break;
		case 64:
			bufLen = len*3/4+1;
			break;
		default:
			assert(0);
			return false;
		}
		std::vector<unsigned char> buf(bufLen);

 		RAND_bytes(&buf[0], bufLen);

		switch(base)
		{
		case 16:
			return crypto::mkstr16(&buf[0], bufLen, rval, len);
		case 64:
			return crypto::mkstr64(&buf[0], bufLen, rval, len);
		default:
			assert(0);
			return false;
		}

		assert(0);
		return false;
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

		size_t range = (size_t)(to - from);
		size_t bits = 0;
		size_t tmp = range;
		while(tmp)
		{
			bits++;
			tmp = tmp>>1;
		}

		std::vector<unsigned char> sbuf(bits/8+1);
		RAND_bytes(&sbuf[0], sbuf.size());

		int val = 0;
		for(size_t i(0); i<sbuf.size(); i++)
		{
			val = val<<8;
			val |= sbuf[i];
		}
		val = val % range;
		val = val+from;

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

		jsdouble val = double(llval) / ULLONG_MAX;

		val = val*(to-from) + from;

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
}
