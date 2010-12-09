#include "stdafx.h"
#include "crypto/symBase.hpp"
#include "crypto/utils.hpp"
#include "utils/utf8.h"

namespace ccms {namespace crypto
{

	//////////////////////////////////////////////////////////////////////////
	SymBase::SymBase(const char *name)
		: JsObject(true, name)
	{
		(JSExceptionReporter)jsRegisterMeth("generateKey", &SymBase::call_generateKey, 0);

		(JSExceptionReporter)jsRegisterMeth("encode", &SymBase::call_encode, 2);
		(JSExceptionReporter)jsRegisterMeth("decode", &SymBase::call_decode, 2);

		(JSExceptionReporter)jsRegisterMeth("encodeJson", &SymBase::call_encodeJson, 2);
		(JSExceptionReporter)jsRegisterMeth("decodeJson", &SymBase::call_decodeJson, 2);
	}

	//////////////////////////////////////////////////////////////////////////
	SymBase::~SymBase()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool SymBase::call_generateKey(uintN argc, jsval *argv, jsval *rval)
	{
		size_t len = 0;
		
		if(argc)
		{
			int32 i;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "i", &i)) return false;

			if(i<8 || i>65536)
			{
				JS_ReportError(ecx()->_jsCx, "%s.generateKey: wrong key len", _strVal.c_str());
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
	bool SymBase::call_encode(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 2)
		{
			JS_ReportError(ecx()->_jsCx, "%s.encode must be called with 2 args", _strVal.c_str());
			return false;
		}
		char *key;
		char *msg;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &key, &msg)) return false;

		size_t msgLength = strlen(msg);

		std::vector<unsigned char> res(msgLength+4);
		size_t resLength = res.size();

		if(!this->encode(
			key,
			(unsigned char *)msg, msgLength,
			&res[0], resLength))
		{
			*rval = JSVAL_VOID;
			return true;
		}

		return mkstr64(&res[0], resLength, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool SymBase::call_decode(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 2)
		{
			JS_ReportError(ecx()->_jsCx, "%s.encode must be called with 2 args", _strVal.c_str());
			return false;
		}
		char *key;
		char *c64;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &key, &c64)) return false;

		size_t c64Length = strlen(c64);
		std::vector<unsigned char> c(c64Length*3/4);
		if(!mk64str((unsigned char *)c64, c64Length, &c[0]))
		{
			*rval = JSVAL_VOID;
			return true;
		}

		if(c.size() < 4)
		{
			*rval = JSVAL_VOID;
			return true;
		}

		std::vector<unsigned char> res(c.size());
		size_t resLength = res.size();

		if(!this->decode(
			key,
			(unsigned char *)&c[0], c.size(),
			&res[0], resLength))
		{
			*rval = JSVAL_VOID;
			return true;
		}
		res.resize(resLength);

		if(!utf8::is_valid(res.begin(), res.end()))
		{
			*rval = JSVAL_VOID;
			return true;
		}

		JSString *str = JS_NewStringCopyN(ecx()->_jsCx, (char *)&res[0], res.size());
		if(!str)
		{
			return false;
		}

		*rval = STRING_TO_JSVAL(str);
		return true;
	}

	namespace impl
	{
		JSBool stringifyCallback(const jschar *buf, uint32 len, void *data)
		{
			std::vector<unsigned char> &sum = *(std::vector<unsigned char> *)data;
			std::back_insert_iterator<std::vector<unsigned char> > bi(sum);

			utf8::unchecked::utf16to8(buf, buf+len, bi);

			return JS_TRUE;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool SymBase::call_encodeJson(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 2)
		{
			JS_ReportError(ecx()->_jsCx, "%s.encodeJs must be called with 2 args", _strVal.c_str());
			return false;
		}
		char *key;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &key)) return false;

		std::vector<unsigned char> msg;
		if(!JS_Stringify(ecx()->_jsCx, argv+1, NULL, JSVAL_NULL, impl::stringifyCallback, &msg)) return false;


		std::vector<unsigned char> res(msg.size()+4);
		size_t resLength = res.size();

		if(!this->encode(
			key,
			&msg[0], msg.size(),
			&res[0], resLength))
		{
			*rval = JSVAL_VOID;
			return true;
		}

		return mkstr64(&res[0], resLength, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool SymBase::call_decodeJson(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 2)
		{
			JS_ReportError(ecx()->_jsCx, "%s.encode must be called with 2 args", _strVal.c_str());
			return false;
		}
		char *key;
		char *c64;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &key, &c64)) return false;

		size_t c64Length = strlen(c64);
		std::vector<unsigned char> c(c64Length*3/4);
		if(!mk64str((unsigned char *)c64, c64Length, &c[0]))
		{
			*rval = JSVAL_VOID;
			return true;
		}

		if(c.size() < 4)
		{
			*rval = JSVAL_VOID;
			return true;
		}


		std::vector<unsigned char> res8(c.size());
		size_t res8Length = res8.size();

		if(!this->decode(
			key,
			(unsigned char *)&c[0], c.size(),
			&res8[0], res8Length))
		{
			*rval = JSVAL_VOID;
			return true;
		}
		res8.resize(res8Length);

		std::vector<jschar> res16;
		try
		{
			std::back_insert_iterator< std::vector<jschar> > bi(res16);
			utf8::utf8to16(res8.begin(), res8.end(), bi);
		}
		catch(...)
		{
			*rval = JSVAL_VOID;
			return true;
		}


		JSONParser *jp = JS_BeginJSONParse(ecx()->_jsCx, rval);
		if(!jp)
		{
			return false;
		}

		if(!JS_ConsumeJSONText(ecx()->_jsCx, jp, &res16[0], res16.size()))
		{
			if(JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ClearPendingException(ecx()->_jsCx);
			}
			*rval = JSVAL_VOID;
			return true;
		}

		jsval jsv = JSVAL_TRUE;
		if(!JS_FinishJSONParse(ecx()->_jsCx, jp, jsv)) return false;

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	size_t SymBase::keyBits()
	{
		//для base64 очень приятны размеры, кратные 24
		return 96;
	}

}}
