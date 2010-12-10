#include "stdafx.h"
#include "crypto/aes.hpp"
#include <openssl/sha.h>
#include <openssl/aes.h>

namespace ccms{ namespace crypto{

	//////////////////////////////////////////////////////////////////////////
	Aes::Aes()
		: SymBase("Aes")
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	Aes::~Aes()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	size_t Aes::keyBits()
	{
		return 256;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Aes::encrypt(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		AES_KEY akey;
		int i;

		switch(keyLength*8)
		{
		case 256:
			i = AES_set_encrypt_key(key, 256, &akey);
			break;
		default:
			{
				unsigned char buf[256/8];
				SHA256_CTX ctx;
				SHA256_Init(&ctx);
				SHA256_Update(&ctx, key, keyLength);
				SHA256_Final(buf, &ctx);

				i = AES_set_encrypt_key(buf, 256, &akey);
			}
			break;
		}

		unsigned char ivec[AES_BLOCK_SIZE] = {};
		//unsigned char ecount_buf[AES_BLOCK_SIZE] = {};

		int num = 0;

		//AES_ctr128_encrypt(msg, res+4, msgLen, &akey, ivec, ecount_buf, &num);
		AES_cfb8_encrypt(in, out, len, &akey, ivec, &num, AES_ENCRYPT);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Aes::decrypt(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		AES_KEY akey;
		int i;

		switch(keyLength*8)
		{
		case 256:
			i = AES_set_encrypt_key(key, 256, &akey);
			break;
		default:
			{
				unsigned char buf[256/8];
				SHA256_CTX ctx;
				SHA256_Init(&ctx);
				SHA256_Update(&ctx, key, keyLength);
				SHA256_Final(buf, &ctx);

				i = AES_set_encrypt_key(buf, 256, &akey);
			}
			break;
		}

		unsigned char ivec[AES_BLOCK_SIZE] = {};
		//unsigned char ecount_buf[AES_BLOCK_SIZE] = {};

		int num = 0;
		//AES_ctr128_encrypt(msg, res, msgLen, &akey, ivec, ecount_buf, &num);
		AES_cfb8_encrypt(in, out, len, &akey, ivec, &num, AES_DECRYPT);

		return true;
	}

}}
