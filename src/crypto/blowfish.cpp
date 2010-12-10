#include "stdafx.h"

#ifndef OPENSSL_NO_BLOWFISH

#include "crypto/blowfish.hpp"
#include <openssl/blowfish.h>
#include "utils/crc32.hpp"

namespace ccms{ namespace crypto{

	//////////////////////////////////////////////////////////////////////////
	Blowfish::Blowfish()
		: SymBase("Blowfish")
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	Blowfish::~Blowfish()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Blowfish::encode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		BF_KEY bfkey;
		if(keyLength < 32/8)
		{
			boost::int32_t crc32 = Crc32(key, keyLength);
			BF_set_key(&bfkey, 4, (unsigned char *)&crc32);
		}
		else if(keyLength > 512/8)
		{
			unsigned char buf[512/8];
			SHA512_CTX ctx;
			SHA512_Init(&ctx);
			SHA512_Update(&ctx, key, keyLength);
			SHA512_Final(buf, &ctx);

			BF_set_key(&bfkey, 512/8, buf);
		}
		else
		{
			BF_set_key(&bfkey, keyLength, key);
		}

		unsigned char ivec[BF_BLOCK] = {};
		int num = 0;
		BF_cfb64_encrypt(in, out, len, &bfkey, ivec, &num, BF_ENCRYPT);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Blowfish::decode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		BF_KEY bfkey;
		if(keyLength < 32/8)
		{
			boost::int32_t crc32 = Crc32(key, keyLength);
			BF_set_key(&bfkey, 4, (unsigned char *)&crc32);
		}
		else if(keyLength > 512/8)
		{
			unsigned char buf[512/8];
			SHA512_CTX ctx;
			SHA512_Init(&ctx);
			SHA512_Update(&ctx, key, keyLength);
			SHA512_Final(buf, &ctx);

			BF_set_key(&bfkey, 512/8, buf);
		}
		else
		{
			BF_set_key(&bfkey, keyLength, key);
		}

		unsigned char ivec[BF_BLOCK] = {};
		int num = 0;
		BF_cfb64_encrypt(in, out, len, &bfkey, ivec, &num, BF_DECRYPT);

		return true;
	}

}}

#endif
