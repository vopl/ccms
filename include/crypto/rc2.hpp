#ifndef _ccms_crypto_rc2_hpp_
#define _ccms_crypto_rc2_hpp_

#include "crypto/symBase.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class Rc2
		: public SymBase
	{
	public:
		Rc2();
		~Rc2();

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

