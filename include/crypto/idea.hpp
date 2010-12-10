#ifndef _ccms_crypto_idea_hpp_
#define _ccms_crypto_idea_hpp_

#include "crypto/symBase.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class Idea
		: public SymBase
	{
	public:
		Idea();
		~Idea();

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

