#ifndef _ccms_crypto_des_hpp_
#define _ccms_crypto_des_hpp_

#include "crypto/syncBase.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class Des
		: public SyncBase
	{
	public:
		Des();
		~Des();

		virtual bool encode(
			const char *key, 
			const unsigned char *msg, size_t msgLen,
			unsigned char *res, size_t &resLen);

		virtual bool decode(
			const char *key, 
			const unsigned char *msg, size_t msgLen,
			unsigned char *res, size_t &resLen);

	};
}}
#endif

