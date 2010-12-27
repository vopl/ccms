#include "stdafx.h"
#include "crypto/crypto.hpp"
#include "crypto/hash.hpp"
#include "crypto/rand.hpp"

#ifndef OPENSSL_NO_DES
#include "crypto/des.hpp"
#endif

#ifndef OPENSSL_NO_AES
#include "crypto/aes.hpp"
#endif

#ifndef OPENSSL_NO_BLOWFISH
#include "crypto/blowfish.hpp"
#endif

#ifndef OPENSSL_NO_CAST
#include "crypto/cast.hpp"
#endif

#ifndef OPENSSL_NO_IDEA
#include "crypto/idea.hpp"
#endif

#ifndef OPENSSL_NO_RC2
#include "crypto/rc2.hpp"
#endif

#ifndef OPENSSL_NO_RC4
#include "crypto/rc4.hpp"
#endif


namespace ccms {namespace crypto
{

	//////////////////////////////////////////////////////////////////////////
	Crypto::Crypto()
		: JsObject(true, "Crypto")
	{
		(JSExceptionReporter)jsRegisterProp("Rand", (new Rand)->thisJsval(), true);
		(JSExceptionReporter)jsRegisterProp("Hash", (new Hash)->thisJsval(), true);

#ifndef OPENSSL_NO_DES
		(JSExceptionReporter)jsRegisterProp("Des", (new Des)->thisJsval(), true);
#endif

#ifndef OPENSSL_NO_AES
		(JSExceptionReporter)jsRegisterProp("Aes", (new Aes)->thisJsval(), true);
#endif

#ifndef OPENSSL_NO_BLOWFISH
		(JSExceptionReporter)jsRegisterProp("Blowfish", (new Blowfish)->thisJsval(), true);
#endif

#ifndef OPENSSL_NO_CAST
		(JSExceptionReporter)jsRegisterProp("Cast", (new Cast)->thisJsval(), true);
#endif

#ifndef OPENSSL_NO_IDEA
		(JSExceptionReporter)jsRegisterProp("Idea", (new Idea)->thisJsval(), true);
#endif

#ifndef OPENSSL_NO_RC2
		(JSExceptionReporter)jsRegisterProp("Rc2", (new Rc2)->thisJsval(), true);
#endif

#ifndef OPENSSL_NO_RC4
		(JSExceptionReporter)jsRegisterProp("Rc4", (new Rc4)->thisJsval(), true);
#endif

	}

	//////////////////////////////////////////////////////////////////////////
	Crypto::~Crypto()
	{

	}

}}
