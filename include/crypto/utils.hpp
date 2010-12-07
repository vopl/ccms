#ifndef _ccms_crypto_utils_http_
#define _ccms_crypto_utils_http_

#include <js/jsapi.h>

namespace ccms
{
	namespace crypto
	{
		bool mkstr16(const unsigned char *sum, size_t len, jsval *rval, size_t maxOutLen=(size_t)-1);
		bool mkstr64(const unsigned char *sum, size_t len, jsval *rval, size_t maxOutLen=(size_t)-1);
	}
}
#endif
