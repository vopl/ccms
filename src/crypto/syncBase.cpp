#include "stdafx.h"
#include "crypto/syncBase.hpp"
#include "crypto/utils.hpp"

namespace ccms {namespace crypto
{

	//////////////////////////////////////////////////////////////////////////
	SyncBase::SyncBase(const char *name)
		: JsObject(true, name)
	{
		(JSExceptionReporter)jsRegisterMeth("generateKey", &SyncBase::call_generateKey, 0);
		(JSExceptionReporter)jsRegisterMeth("encode", &SyncBase::call_encode, 2);
		(JSExceptionReporter)jsRegisterMeth("decode", &SyncBase::call_encode, 2);
	}

	//////////////////////////////////////////////////////////////////////////
	SyncBase::~SyncBase()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool SyncBase::call_generateKey(uintN argc, jsval *argv, jsval *rval)
	{
		size_t len = 0;
		
		if(argc)
		{
			int32 i;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "i", &i)) return false;

			if(i<8 || i>65536)
			{
				JS_ReportError(ecx()->_jsCx, "SyncBase.generateKey: wrong key len");
				return false;
			}
			len = (size_t)i;
		}
		else
		{
			len = this->keyBits();
		}

		len /= 8;

		std::vector<unsigned char> sbuf(len);
		RAND_bytes(&sbuf[0], len);

		return mkstr64(&sbuf[0], len, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool SyncBase::call_encode(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 2)
		{
			JS_ReportError(ecx()->_jsCx, "SyncBase.encode must be called with 2 args");
			return false;
		}
		JSString *key, *msg;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "SS", &key, &msg)) return false;

		jschar *keyChars = JS_GetStringChars(key);
		size_t keyLength = JS_GetStringLength(key);

		jschar *msgChars = JS_GetStringChars(msg);
		size_t msgLength = JS_GetStringLength(msg);

		assert(0);
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool SyncBase::call_decode(uintN argc, jsval *argv, jsval *rval)
	{
		assert(0);
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	size_t SyncBase::keyBits()
	{
		//для base64 очень приятны размеры, кратные 24
		return 96;
	}

}}
