#include "stdafx.h"

#ifndef OPENSSL_NO_CAST

#include "crypto/cast.hpp"
#include <openssl/cast.h>
#include <openssl/md5.h>

namespace ccms{ namespace crypto{

	//////////////////////////////////////////////////////////////////////////
	Cast::Cast()
		: SymBase("Cast")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Cast::~Cast()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cast::encode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		CAST_KEY ckey;
		CAST_set_key(&ckey, keyLength, key);

		unsigned char ivec[CAST_BLOCK] = {};
		int num = 0;
		CAST_cfb64_encrypt(in, out, len, &ckey, ivec, &num, CAST_ENCRYPT);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Cast::decode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		CAST_KEY ckey;
		CAST_set_key(&ckey, keyLength, key);

		unsigned char ivec[CAST_BLOCK] = {};
		int num = 0;
		CAST_cfb64_encrypt(in, out, len, &ckey, ivec, &num, CAST_DECRYPT);

		return true;
	}

}}

#endif
