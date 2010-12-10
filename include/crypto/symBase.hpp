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

		bool call_encrypt(uintN argc, jsval *argv, jsval *rval);
		bool call_decrypt(uintN argc, jsval *argv, jsval *rval);

		bool call_encryptJson(uintN argc, jsval *argv, jsval *rval);
		bool call_decryptJson(uintN argc, jsval *argv, jsval *rval);

	private:
		virtual size_t keyBits();
		virtual bool encrypt(
			const unsigned char *key, size_t keyLength, 
			const unsigned char *in, 
			unsigned char *out,
			size_t len)=0;

		virtual bool decrypt(
			const unsigned char *key, size_t keyLength, 
			const unsigned char *in, 
			unsigned char *out,
			size_t len)=0;
	};
}}
#endif

