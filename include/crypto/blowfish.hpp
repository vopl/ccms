#ifndef _ccms_crypto_blowfish_hpp_
#define _ccms_crypto_blowfish_hpp_

#include "crypto/symBase.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class Blowfish
		: public SymBase
	{
	public:
		Blowfish();
		~Blowfish();

		virtual bool encrypt(
			const unsigned char *key, size_t keyLength, 
			const unsigned char *in,
			unsigned char *out,
			size_t len);

		virtual bool decrypt(
			const unsigned char *key, size_t keyLength, 
			const unsigned char *in,
			unsigned char *out,
			size_t len);
	};
}}
#endif

