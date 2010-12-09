#include "stdafx.h"
#include "crypto/aes.hpp"
#include <openssl/sha.h>
#include <openssl/aes.h>

namespace ccms{ namespace crypto{

	//////////////////////////////////////////////////////////////////////////
	Aes::Aes()
		: SyncBase("Aes")
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
	bool Aes::encode(
		const char *key, 
		const unsigned char *msg, size_t msgLen,
		unsigned char *res, size_t &resLen)
	{
		size_t keyLength = strlen(key);
		AES_KEY akey;
		int i;

		switch(keyLength*8)
		{
		case 256:
			i = AES_set_encrypt_key((const unsigned char*)key, 256, &akey);
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
		unsigned char ecount_buf[AES_BLOCK_SIZE] = {};

		unsigned int num = 0;
		AES_ctr128_encrypt(msg, res, msgLen, &akey, ivec, ecount_buf, &num);

		resLen = msgLen;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Aes::decode(
		const char *key, 
		const unsigned char *msg, size_t msgLen,
		unsigned char *res, size_t &resLen)
	{
		return encode(key, msg, msgLen, res, resLen);
	}

}}