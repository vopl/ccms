#include "stdafx.h"
#include "crypto/rc4.hpp"
#include <openssl/rc4.h>

namespace ccms{ namespace crypto{

	//////////////////////////////////////////////////////////////////////////
	Rc4::Rc4()
		: SymBase("Rc4")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Rc4::~Rc4()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rc4::encrypt(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		RC4_KEY rc4key;
		RC4_set_key(&rc4key, keyLength, key);

		RC4(&rc4key, len, in, out);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rc4::decrypt(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		RC4_KEY rc4key;
		RC4_set_key(&rc4key, keyLength, key);

		RC4(&rc4key, len, in, out);
		return true;
	}

}}
