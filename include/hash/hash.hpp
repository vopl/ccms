#ifndef _ccms_hash_hpp_
#define _ccms_hash_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
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

	private:
		bool mkstr(unsigned char * const sum, size_t len, jsval *rval);
	};

	typedef JsPtr<Hash> HashPtr;
}
#endif

