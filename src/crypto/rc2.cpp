#include "stdafx.h"

#ifndef OPENSSL_NO_RC2

#include "crypto/rc2.hpp"
#include <openssl/rc2.h>

namespace ccms{ namespace crypto{

	//////////////////////////////////////////////////////////////////////////
	Rc2::Rc2()
		: SymBase("Rc2")
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	Rc2::~Rc2()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rc2::encode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		RC2_KEY rc4key;
		RC2_set_key(&rc4key, keyLength, key, keyLength*8);

		unsigned char ivec[RC2_BLOCK] = {};
		int num=0;
		RC2_cfb64_encrypt(in, out, len, &rc4key, ivec, &num, RC2_ENCRYPT);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Rc2::decode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		RC2_KEY rc4key;
		RC2_set_key(&rc4key, keyLength, key, keyLength*8);

		unsigned char ivec[RC2_BLOCK] = {};
		int num=0;
		RC2_cfb64_encrypt(in, out, len, &rc4key, ivec, &num, RC2_DECRYPT);
		return true;
	}

}}

#endif
