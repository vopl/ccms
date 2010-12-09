#include "stdafx.h"
#include "crypto/crypto.hpp"
#include "crypto/hash.hpp"
#include "crypto/rand.hpp"

#include "crypto/des.hpp"
#include "crypto/aes.hpp"

namespace ccms {namespace crypto
{

	//////////////////////////////////////////////////////////////////////////
	Crypto::Crypto()
		: JsObject(true, "Crypto")
	{
		(JSExceptionReporter)jsRegisterProp("rand", (new Rand)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("hash", (new Hash)->thisJsval(), true);

		JSObject *sync = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		if(!sync)
		{
			(JSExceptionReporter)false;
			return;
		}

		JS_DefineProperty(ecx()->_jsCx, sync, "des", (new Des)->thisJsval(), NULL, NULL, JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT);
		JS_DefineProperty(ecx()->_jsCx, sync, "aes", (new Aes)->thisJsval(), NULL, NULL, JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT);
		(JSExceptionReporter)jsRegisterProp("sync", sync, true);
	}

	//////////////////////////////////////////////////////////////////////////
	Crypto::~Crypto()
	{

	}

}}
