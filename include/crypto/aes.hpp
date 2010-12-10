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

		virtual bool encode(
			const unsigned char *key, size_t keyLength, 
			const unsigned char *in,
			unsigned char *out,
			size_t len);

		virtual bool decode(
			const unsigned char *key, size_t keyLength, 
			const unsigned char *in,
			unsigned char *out,
			size_t len);
	};
}}
#endif

