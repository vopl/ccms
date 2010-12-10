#ifndef _ccms_crypto_aes_hpp_
#define _ccms_crypto_aes_hpp_

#include "crypto/symBase.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class Aes
		: public SymBase
	{
	public:
		Aes();
		~Aes();

		virtual size_t keyBits();

		virtual bool encrypt(
			const char *key, 
			const unsigned char *msg, size_t msgLen,
			unsigned char *res, size_t &resLen);

		virtual bool decrypt(
			const char *key, 
			const unsigned char *msg, size_t msgLen,
			unsigned char *res, size_t &resLen);

	};
}}
#endif

