#include "stdafx.h"
#include "crypto/des.hpp"

#include <openssl/des.h>

namespace ccms{ namespace crypto{

	//////////////////////////////////////////////////////////////////////////
	Des::Des()
		: SyncBase("Des")
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	Des::~Des()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Des::encode(
		const char *key, 
		const unsigned char *msg, size_t msgLen,
		unsigned char *res, size_t &resLen)
	{
		DES_cblock cbkey;
		DES_key_schedule sched;
		DES_string_to_key(key, &cbkey);
		DES_set_key_unchecked(&cbkey, &sched);
		int num = 0;
		DES_ofb64_encrypt(msg, res, msgLen, &sched, &cbkey, &num);

		resLen = msgLen;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Des::decode(
		const char *key, 
		const unsigned char *msg, size_t msgLen,
		unsigned char *res, size_t &resLen)
	{
		return encode(key, msg, msgLen, res, resLen);
	}

}}