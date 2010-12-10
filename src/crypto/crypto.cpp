#include "stdafx.h"
#include "crypto/crypto.hpp"
#include "crypto/hash.hpp"
#include "crypto/rand.hpp"

#include "crypto/des.hpp"
#include "crypto/aes.hpp"
#include "crypto/blowfish.hpp"
#include "crypto/cast.hpp"
#include "crypto/idea.hpp"
#include "crypto/rc2.hpp"
#include "crypto/rc4.hpp"

namespace ccms {namespace crypto
{

	//////////////////////////////////////////////////////////////////////////
	Crypto::Crypto()
		: JsObject(true, "Crypto")
	{
		(JSExceptionReporter)jsRegisterProp("rand", (new Rand)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("hash", (new Hash)->thisJsval(), true);

		(JSExceptionReporter)jsRegisterProp("des", (new Des)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("aes", (new Aes)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("blowfish", (new Blowfish)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("cast", (new Cast)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("idea", (new Idea)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("rc2", (new Rc2)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("rc4", (new Rc4)->thisJsval(), true);
	}

	//////////////////////////////////////////////////////////////////////////
	Crypto::~Crypto()
	{

	}

}}
