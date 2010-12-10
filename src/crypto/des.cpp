#include "stdafx.h"

#ifndef OPENSSL_NO_DES

#include "crypto/des.hpp"
#include <openssl/des.h>

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
	bool Des::encode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		DES_cblock cbkey;
		DES_key_schedule sched;
		DES_string_to_key((const char *)key, &cbkey);
		DES_set_key_unchecked(&cbkey, &sched);

		int num = 0;
		//DES_ofb64_encode(msg, res, msgLen, &sched, &cbkey, &num);
		DES_cfb64_encrypt(in, out, len, &sched, &cbkey, &num, DES_ENCRYPT);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Des::decode(
		const unsigned char *key, size_t keyLength, 
		const unsigned char *in,
		unsigned char *out,
		size_t len)
	{
		DES_cblock cbkey;
		DES_key_schedule sched;
		DES_string_to_key((const char *)key, &cbkey);
		DES_set_key_unchecked(&cbkey, &sched);

		int num = 0;
		DES_cfb64_encrypt(in, out, len, &sched, &cbkey, &num, DES_DECRYPT);

		return true;
	}

}}

#endif
