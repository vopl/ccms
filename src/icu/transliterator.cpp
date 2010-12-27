#include "stdafx.h"
#include "icu/transliterator.hpp"

namespace ccms{namespace icu{

	//////////////////////////////////////////////////////////////////////////
	Transliterator::Transliterator()
		: JsObject(true, "Transliterator")
		, _transliterator(NULL)
	{
		(JSExceptionReporter)jsRegisterProp("id", &Transliterator::xetter_id, false);

		(JSExceptionReporter)jsRegisterMeth("transliterate", &Transliterator::call_transliterate, 1);
	}
	//////////////////////////////////////////////////////////////////////////
	Transliterator::~Transliterator()
	{
		if(_transliterator)
		{
			delete _transliterator;
			_transliterator = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool Transliterator::xetter_id(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!_transliterator)
			{
				*vp = JSVAL_VOID;
				return true;
			}

			::icu::UnicodeString us = _transliterator->getID();

			std::string sink;
			us.toUTF8String(sink);

			JSString *str = JS_NewStringCopyN(ecx()->_jsCx, sink.data(), sink.size());
			
			*vp = STRING_TO_JSVAL(str);
			return true;
		}
		else
		{
			char *s;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &s)) return false;

			::icu::UnicodeString us = ::icu::UnicodeString::fromUTF8(s);

			UParseError parseError={};
			UErrorCode status=U_ZERO_ERROR;
			::icu::Transliterator *t = ::icu::Transliterator::createInstance(us, UTRANS_FORWARD, parseError, status);
			
			if(!t)
			{
				JS_ReportError(ecx()->_jsCx, "transliterator failed to create instance with id %s, %s(%d), line %d offset %d", s, u_errorName(status), (int)status, parseError.line, parseError.offset);
				return false;
			}

			if(_transliterator)
			{
				delete _transliterator;
			}
			_transliterator = t;

			return true;
		}

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	bool Transliterator::call_transliterate(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "transliterator.transliterate must be called with 1 arg");
			return false;
		}

		if(!_transliterator)
		{
			JS_ReportError(ecx()->_jsCx, "transliterator.transliterate id is not setted");
			return false;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &s)) return false;
		::icu::UnicodeString us = ::icu::UnicodeString::fromUTF8(s);

		_transliterator->transliterate(us);


		std::string sink;
		us.toUTF8String(sink);

		JSString *str = JS_NewStringCopyN(ecx()->_jsCx, sink.data(), sink.size());

		*rval = STRING_TO_JSVAL(str);
		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	TransliteratorConstructor::TransliteratorConstructor()
		: JsObject(true, "TransliteratorConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool TransliteratorConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		Transliterator *t = new Transliterator;

		if(argc)
		{
			if(!t->xetter_id(argv, false)) return JS_FALSE;
		}

		*rval = t->thisJsval();
		return true;
	}
}}
