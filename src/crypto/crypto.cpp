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

		(JSExceptionReporter)jsRegisterProp("des", (new Des)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("aes", (new Aes)->thisJsval(), true);
	}

	//////////////////////////////////////////////////////////////////////////
	Crypto::~Crypto()
	{

	}

}}
