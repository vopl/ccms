#include "stdafx.h"
#include "hash/hash.hpp"
#include "utils/crc32.hpp"
#include <openssl/sha.h>
#include <openssl/md4.h>
#include <openssl/md5.h>

namespace ccms
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
	}

	//////////////////////////////////////////////////////////////////////////
	Hash::~Hash()
	{

	}




	//////////////////////////////////////////////////////////////////////////
#define ONE_METH(name, cname, mname, len)\
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
		return mkstr(res, len/8, rval);\
	}//\

	ONE_METH(crc32, CRC32, CRC32, 32)
	ONE_METH(md4, MD4, MD4, 128)
	ONE_METH(md5, MD5, MD5, 128)
	ONE_METH(sha0, SHA, SHA, 160)
	ONE_METH(sha1, SHA, SHA1, 160)
	ONE_METH(sha224, SHA256, SHA224, 224)
	ONE_METH(sha256, SHA256, SHA256, 256)
	ONE_METH(sha384, SHA512, SHA384, 384)
	ONE_METH(sha512, SHA512, SHA512, 512)


	//////////////////////////////////////////////////////////////////////////
	namespace impl
	{
		static const char Hash_hex[] = {"0123456789abcdef"};
	}
	bool Hash::mkstr(unsigned char * const sum, size_t len, jsval *rval)
	{
		char sbuf[1024];

		for(size_t i(0); i<len; i++)
		{
			sbuf[i*2+0] = impl::Hash_hex[sum[i]>>4];
			sbuf[i*2+1] = impl::Hash_hex[sum[i]&0xf];
		}

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, sbuf, len*2));
		return true;
	}

}
