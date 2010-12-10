#include "stdafx.h"
#include "crypto/des.hpp"
#include <openssl/des.h>

#include "utils/crc32.hpp"

namespace ccms{ namespace crypto{

	//////////////////////////////////////////////////////////////////////////
	Des::Des()
		: SymBase("Des")
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	Des::~Des()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Des::encrypt(
		const char *key, 
		const unsigned char *msg, size_t msgLen,
		unsigned char *res, size_t &resLen)
	{
		assert(msgLen+4 == resLen);
		DES_cblock cbkey;
		DES_key_schedule sched;
		DES_string_to_key(key, &cbkey);
		DES_set_key_unchecked(&cbkey, &sched);

		int num = 0;

		boost::int32_t crc32 = Crc32((const char *)msg, msgLen);
		//DES_ofb64_encrypt((unsigned char *)&crc32, res, 4, &sched, &cbkey, &num);
		DES_cfb64_encrypt((unsigned char *)&crc32, res, 4, &sched, &cbkey, &num, DES_ENCRYPT);
		//DES_ofb64_encrypt(msg, res+4, msgLen, &sched, &cbkey, &num);
		DES_cfb64_encrypt(msg, res+4, msgLen, &sched, &cbkey, &num, DES_ENCRYPT);

		resLen = msgLen+4;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Des::decrypt(
		const char *key, 
		const unsigned char *msg, size_t msgLen,
		unsigned char *res, size_t &resLen)
	{
		assert(msgLen == resLen);
		DES_cblock cbkey;
		DES_key_schedule sched;
		DES_string_to_key(key, &cbkey);
		DES_set_key_unchecked(&cbkey, &sched);

		int num = 0;
// 		DES_ofb64_encrypt(msg, res, msgLen, &sched, &cbkey, &num);
		DES_cfb64_encrypt(msg, res, msgLen, &sched, &cbkey, &num, DES_DECRYPT);

		boost::int32_t crc32 = Crc32((const char *)res+4, msgLen-4);

		if(crc32 != *(const boost::int32_t *)(res))
		{
			return false;
		}

		resLen = msgLen;

		return true;
	}

}}
