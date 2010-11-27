#include "stdafx.h"
#include "router/log.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Log::Log()
	{
		(JSExceptionReporter)jsRegisterMeth("open", &Log::call_open, 1);
		(JSExceptionReporter)jsRegisterMeth("print", &Log::call_print, 1);
		(JSExceptionReporter)jsRegisterMeth("close", &Log::call_close, 1);
	}

	//////////////////////////////////////////////////////////////////////////
	Log::~Log()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool Log::call_open(uintN argc, jsval *argv, jsval *rval)
	{
		_out.close();
		_out.clear();

		if(argc != 1)
		{
			JS_ReportError(ecx()->_jsCx, "Log.open must be called eith 1 arg");
			return false;
		}

		const char *fname;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &fname)) return false;
		_out.open(fname, std::ios::app|std::ios::binary);
		if(!_out)
		{
			_out.close();
			_out.clear();
			JS_ReportError(ecx()->_jsCx, "Log.open failed to open %s", fname);
			return false;
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Log::call_print(uintN argc, jsval *argv, jsval *rval)
	{
		if(!_out)
		{
			JS_ReportError(ecx()->_jsCx, "Log.print bad stream");
			return false;
		}

		JSContext *cx = ecx()->_jsCx;

		for (uintN  i = 0; i < argc; i++)
		{
			if(JSVAL_IS_STRING(argv[i]))
			{
				JSString *str = JSVAL_TO_STRING(argv[i]);
				size_t len = JS_GetStringLength(str);

				if(len)
				{
					jschar *chars = JS_GetStringChars(str);
					size_t dstLen = len*2;
					std::vector<char> dst(dstLen);

					(JSExceptionReporter)JS_EncodeCharacters(cx, chars, len, &dst[0], &dstLen);
					_out.write(&dst[0], dstLen);
				}
			}
			else
			{
				char *s;
				if(!JS_ConvertArguments(cx, 1, argv+i, "s", &s))
				{
					if(JS_IsExceptionPending(cx))
					{
						JS_ReportPendingException(cx);
						JS_ClearPendingException(cx);
					}
					return JS_FALSE;
				}
				_out.rdbuf()->sputn(s, strlen(s));
			}
		}
		_out.flush();

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Log::call_close(uintN argc, jsval *argv, jsval *rval)
	{
		_out.close();
		_out.clear();
		*rval = JSVAL_VOID;
		return true;
	}


}
