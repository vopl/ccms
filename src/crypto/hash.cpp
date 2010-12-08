#include "stdafx.h"
#include "crypto/hash.hpp"
#include "utils/crc32.hpp"
#include <openssl/sha.h>
#include <openssl/md4.h>
#include <openssl/md5.h>

#include "crypto/utils.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	Hash::Hash()
		: JsObject(true, "Hash")
	{
		(JSExceptionReporter)jsRegisterMeth("crc32", &Hash::call_crc32, 1);
		(JSExceptionReporter)jsRegisterMeth("md4", &Hash::call_md4, 1);
		(JSExceptionReporter)jsRegisterMeth("md5", &Hash::call_md5, 1);
		(JSExceptionReporter)jsRegisterMeth("sha0", &Hash::call_sha0, 1);
		(JSExceptionReporter)jsRegisterMeth("sha1", &Hash::call_sha1, 1);
		(JSExceptionReporter)jsRegisterMeth("sha224", &Hash::call_sha224, 1);
		(JSExceptionReporter)jsRegisterMeth("sha256", &Hash::call_sha256, 1);
		(JSExceptionReporter)jsRegisterMeth("sha384", &Hash::call_sha384, 1);
		(JSExceptionReporter)jsRegisterMeth("sha512", &Hash::call_sha512, 1);

		(JSExceptionReporter)jsRegisterMeth("crc32_", &Hash::call_crc32_, 1);
		(JSExceptionReporter)jsRegisterMeth("md4_", &Hash::call_md4_, 1);
		(JSExceptionReporter)jsRegisterMeth("md5_", &Hash::call_md5_, 1);
		(JSExceptionReporter)jsRegisterMeth("sha0_", &Hash::call_sha0_, 1);
		(JSExceptionReporter)jsRegisterMeth("sha1_", &Hash::call_sha1_, 1);
		(JSExceptionReporter)jsRegisterMeth("sha224_", &Hash::call_sha224_, 1);
		(JSExceptionReporter)jsRegisterMeth("sha256_", &Hash::call_sha256_, 1);
		(JSExceptionReporter)jsRegisterMeth("sha384_", &Hash::call_sha384_, 1);
		(JSExceptionReporter)jsRegisterMeth("sha512_", &Hash::call_sha512_, 1);

	}

	//////////////////////////////////////////////////////////////////////////
	Hash::~Hash()
	{

	}




	//////////////////////////////////////////////////////////////////////////
#define ONE_METH(name, cname, mname, len, outBits)\
	bool Hash::call_##name(uintN argc, jsval *argv, jsval *rval)\
	{\
		cname##_CTX ctx;\
		mname##_Init(&ctx);\
		for(uintN i(0); i<argc; i++)\
		{\
			char *s;\
			JS_ConvertArguments(ecx()->_jsCx, 1, argv+i, "s", &s)?true:false;\
			mname##_Update(&ctx, s, strlen(s));\
		}\
		unsigned char res[len/8];\
		mname##_Final(res, &ctx);\
		return crypto::mkstr##outBits(res, len/8, rval);\
	}//\

	ONE_METH(crc32, CRC32, CRC32, 32, 16)
	ONE_METH(md4, MD4, MD4, 128, 16)
	ONE_METH(md5, MD5, MD5, 128, 16)
	ONE_METH(sha0, SHA, SHA, 160, 16)
	ONE_METH(sha1, SHA, SHA1, 160, 16)
	ONE_METH(sha224, SHA256, SHA224, 224, 16)
	ONE_METH(sha256, SHA256, SHA256, 256, 16)
	ONE_METH(sha384, SHA512, SHA384, 384, 16)
	ONE_METH(sha512, SHA512, SHA512, 512, 16)

	ONE_METH(crc32_, CRC32, CRC32, 32, 64)
	ONE_METH(md4_, MD4, MD4, 128, 64)
	ONE_METH(md5_, MD5, MD5, 128, 64)
	ONE_METH(sha0_, SHA, SHA, 160, 64)
	ONE_METH(sha1_, SHA, SHA1, 160, 64)
	ONE_METH(sha224_, SHA256, SHA224, 224, 64)
	ONE_METH(sha256_, SHA256, SHA256, 256, 64)
	ONE_METH(sha384_, SHA512, SHA384, 384, 64)
	ONE_METH(sha512_, SHA512, SHA512, 512, 64)
}}
