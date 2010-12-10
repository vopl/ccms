#ifndef _ccms_crypto_des_hpp_
#define _ccms_crypto_des_hpp_

#include "crypto/symBase.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class Des
		: public SymBase
	{
	public:
		Des();
		~Des();

		virtual bool encrypt(
			const char *key, 
			const unsigned char *in,
			unsigned char *out,
			size_t len);

		virtual bool decrypt(
			const char *key, 
			const unsigned char *in,
			unsigned char *out,
			size_t len);

	};
}}
#endif

