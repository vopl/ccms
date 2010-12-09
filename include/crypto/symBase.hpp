#ifndef _ccms_crypto_symBase_hpp_
#define _ccms_crypto_symBase_hpp_

#include "scripter/jsObject.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class SymBase
		: public JsObject
	{
	public:
		SymBase(const char *name);
		~SymBase();

	public:
		bool call_generateKey(uintN argc, jsval *argv, jsval *rval);

		bool call_encode(uintN argc, jsval *argv, jsval *rval);
		bool call_decode(uintN argc, jsval *argv, jsval *rval);

		bool call_encodeJson(uintN argc, jsval *argv, jsval *rval);
		bool call_decodeJson(uintN argc, jsval *argv, jsval *rval);

	private:
		virtual size_t keyBits();
		virtual bool encode(
			const char *key, 
			const unsigned char *msg, size_t msgLen,
			unsigned char *res, size_t &resLen)=0;

		virtual bool decode(
			const char *key, 
			const unsigned char *msg, size_t msgLen,
			unsigned char *res, size_t &resLen)=0;
	};
}}
#endif

