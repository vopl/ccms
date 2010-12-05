#include "stdafx.h"
#include "hash/hash.hpp"
#include "utils/crc32.hpp"
#include <openssl/sha.h>
#include <openssl/md4.h>
#include <openssl/md5.h>

#define MAX_HASHBITS 512

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
		return mkstr##outBits(res, len/8, rval);\
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


	//////////////////////////////////////////////////////////////////////////
	namespace impl
	{
		static const char Hash_s16[] = {"0123456789abcdef"};
	}
	bool Hash::mkstr16(const unsigned char *sum, size_t len, jsval *rval)
	{
		char sbuf[MAX_HASHBITS/8*2];

		for(size_t i(0); i<len; i++)
		{
			sbuf[i*2+0] = impl::Hash_s16[sum[i]>>4];
			sbuf[i*2+1] = impl::Hash_s16[sum[i]&0xf];
		}

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, sbuf, len*2));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	namespace impl
	{
		static const char Hash_s64[] = 
		{
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789_-"
		};
	}
	bool Hash::mkstr64(const unsigned char *sum_, size_t len, jsval *rval)
	{
		const unsigned char *sum = sum_;
		char sbuf[MAX_HASHBITS/8/3*4+1];

		size_t triplets = len / 3;

		char *out = sbuf;
		for(size_t i(0); i<triplets; i++)
		{
			*out = impl::Hash_s64[(sum[0] & 0xfc) >> 2];
			out++;
			*out = impl::Hash_s64[((sum[0] & 0x03) << 4) + ((sum[1] & 0xf0) >> 4)];
			out++;
			*out = impl::Hash_s64[((sum[1] & 0x0f) << 2) + ((sum[2] & 0xc0) >> 6)];
			out++;
			*out = impl::Hash_s64[sum[2] & 0x3f];
			out++;

			sum += 3;
		}

		switch(len % 3)
		{
		case 0:
			break;
		case 1:
			*out = impl::Hash_s64[(sum[0] & 0xfc) >> 2];
			out++;

// 			*out = impl::Hash_s64[((sum[0] & 0x03) << 4)];
			//зациклить на начало
			*out = impl::Hash_s64[((sum[0] & 0x03) << 4) + ((sum_[0] & 0xf0) >> 4)];
			out++;

			break;
		case 2:
			*out = impl::Hash_s64[(sum[0] & 0xfc) >> 2];
			out++;

			*out = impl::Hash_s64[((sum[0] & 0x03) << 4) + ((sum[1] & 0xf0) >> 4)];
			out++;

//			*out = impl::Hash_s64[((sum[1] & 0x0f) << 2)];
			//зациклить на начало
			*out = impl::Hash_s64[((sum[1] & 0x0f) << 2) + ((sum_[0] & 0xc0) >> 6)];
			out++;

			break;
		}

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, sbuf, out-sbuf));
		return true;
	}

}
