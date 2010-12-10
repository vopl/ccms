#include "stdafx.h"

#ifndef OPENSSL_NO_IDEA

#include "crypto/idea.hpp"
#include <openssl/idea.h>
#include <openssl/md5.h>

namespace ccms{ namespace crypto{

	//////////////////////////////////////////////////////////////////////////
	Idea::Idea()
		: SymBase("Idea")
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	Idea::~Idea()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Idea::encode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		IDEA_KEY_SCHEDULE ikey;
		if(16 == keyLength)
		{
			idea_set_encrypt_key(key, &ikey);
		}
		else
		{
			unsigned char buf[16];
			MD5_CTX ctx;
			MD5_Init(&ctx);
			MD5_Update(&ctx, key, keyLength);
			MD5_Final(buf, &ctx);

			idea_set_encrypt_key(buf, &ikey);
		}


		unsigned char ivec[IDEA_BLOCK] = {};
		int num = 0;
		idea_cfb64_encrypt(in, out, len, &ikey, ivec, &num, IDEA_ENCRYPT);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Idea::decode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		IDEA_KEY_SCHEDULE ikey;
		if(16 == keyLength)
		{
			idea_set_encrypt_key(key, &ikey);
		}
		else
		{
			unsigned char buf[16];
			MD5_CTX ctx;
			MD5_Init(&ctx);
			MD5_Update(&ctx, key, keyLength);
			MD5_Final(buf, &ctx);

			idea_set_encrypt_key(buf, &ikey);
		}

		unsigned char ivec[IDEA_BLOCK] = {};
		int num = 0;
		idea_cfb64_encrypt(in, out, len, &ikey, ivec, &num, IDEA_DECRYPT);

		return true;
	}

}}

#endif
