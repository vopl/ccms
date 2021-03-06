#ifndef _ccms_crypto_hash_hpp_
#define _ccms_crypto_hash_hpp_

#include "scripter/jsObject.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class Hash
		: public JsObject
	{
	public:
		Hash();
		~Hash();

		bool call_crc32(uintN argc, jsval *argv, jsval *rval);
		bool call_md4(uintN argc, jsval *argv, jsval *rval);
		bool call_md5(uintN argc, jsval *argv, jsval *rval);
		bool call_sha0(uintN argc, jsval *argv, jsval *rval);
		bool call_sha1(uintN argc, jsval *argv, jsval *rval);
		bool call_sha224(uintN argc, jsval *argv, jsval *rval);
		bool call_sha256(uintN argc, jsval *argv, jsval *rval);
		bool call_sha384(uintN argc, jsval *argv, jsval *rval);
		bool call_sha512(uintN argc, jsval *argv, jsval *rval);

		bool call_crc32_(uintN argc, jsval *argv, jsval *rval);
		bool call_md4_(uintN argc, jsval *argv, jsval *rval);
		bool call_md5_(uintN argc, jsval *argv, jsval *rval);
		bool call_sha0_(uintN argc, jsval *argv, jsval *rval);
		bool call_sha1_(uintN argc, jsval *argv, jsval *rval);
		bool call_sha224_(uintN argc, jsval *argv, jsval *rval);
		bool call_sha256_(uintN argc, jsval *argv, jsval *rval);
		bool call_sha384_(uintN argc, jsval *argv, jsval *rval);
		bool call_sha512_(uintN argc, jsval *argv, jsval *rval);
	};

	typedef JsPtr<Hash> HashPtr;
}}
#endif

