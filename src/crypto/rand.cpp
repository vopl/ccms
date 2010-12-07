#include "stdafx.h"
#include "crypto/rand.hpp"
#include <openssl/rand.h>
#include "crypto/utils.hpp"
#include "utils/utf8.h"

namespace ccms
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
			return crypto::mkstr16(&buf[0], bufLen, rval, len);
		}
		else
		{
			return crypto::mkstr64(&buf[0], bufLen, rval, len);
		}

		assert(0);
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rand::call_abc(uintN argc, jsval *argv, jsval *rval)
	{
		const int32 *abc = NULL;

		if(!argc)
		{
			JS_ReportError(ecx()->_jsCx , "Rand.abc must be called with 1 or 2 args");
			return false;
		}


		char *abc8;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &abc8)) return false;
		size_t abc8len = strlen(abc8);

		std::vector<utf8::uint32_t> abc32(abc8len);
		try
		{
			std::vector<utf8::uint32_t>::iterator iterEnd = utf8::utf8to32(abc8, abc8+abc8len, abc32.begin());
			abc32.erase(iterEnd, abc32.end());
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
			unsigned long long vll;
			RAND_bytes((unsigned char *)&vll, sizeof(vll));
			res32[i] = abc32[vll % abc32.size()];
		}

		std::vector<utf8::uint8_t> res8(res32.size()*5+1);
		try
		{
			std::vector<utf8::uint8_t>::iterator iterEnd = utf8::utf32to8(res32.begin(), res32.end(), res8.begin());
			res8.erase(iterEnd, res8.end());
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "Rand.abc internal error");
			return false;
		}

		JSString *res = JS_NewStringCopyN(ecx()->_jsCx, (char *)&res8[0], res8.size());
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

	//////////////////////////////////////////////////////////////////////////
	JSBool Rand::onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		return call_double(argc, argv, rval);
	}

}
