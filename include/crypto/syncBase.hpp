#ifndef _ccms_crypto_syncBase_hpp_
#define _ccms_crypto_syncBase_hpp_

#include "scripter/jsObject.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class SyncBase
		: public JsObject
	{
	public:
		SyncBase(const char *name);
		~SyncBase();

	public:
		bool call_generateKey(uintN argc, jsval *argv, jsval *rval);
		bool call_encode(uintN argc, jsval *argv, jsval *rval);
		bool call_decode(uintN argc, jsval *argv, jsval *rval);

	private:
		virtual size_t keyBits();
		virtual bool encode(
			const unsigned char *key, size_t keyLen, 
			const unsigned char *msg, size_t msgLen,
			const unsigned char *res, size_t &resLen)=0;

		virtual bool decode(
			const unsigned char *key, size_t keyLen, 
			const unsigned char *msg, size_t msgLen,
			const unsigned char *res, size_t &resLen)=0;
	};
}}
#endif

