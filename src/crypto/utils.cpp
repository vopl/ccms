#include "stdafx.h"
#include "crypto/utils.hpp"
#include "router/execContext.hpp"

namespace ccms
{
	namespace crypto
	{
		//////////////////////////////////////////////////////////////////////////
		namespace impl
		{
			static const char Hash_s16[] = {"0123456789abcdef"};
		}
		bool mkstr16(const unsigned char *sum, size_t len, jsval *rval, size_t maxOutLen)
		{
			std::vector<char> sbuf(len*2);

			for(size_t i(0); i<len; i++)
			{
				sbuf[i*2+0] = impl::Hash_s16[sum[i]>>4];
				sbuf[i*2+1] = impl::Hash_s16[sum[i]&0xf];
			}

			if(maxOutLen > len*2) maxOutLen = len*2;

			*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, &sbuf[0], maxOutLen));
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
		bool mkstr64(const unsigned char *sum_, size_t len, jsval *rval, size_t maxOutLen)
		{
			const unsigned char *sum = sum_;

			std::vector<char> sbuf(len*4/3+1);

			size_t triplets = len / 3;

			char *out = &sbuf[0];
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

			if(maxOutLen > (size_t)(out-&sbuf[0])) maxOutLen = out-&sbuf[0];

			*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, &sbuf[0], maxOutLen));
			return true;
		}
	}
}
