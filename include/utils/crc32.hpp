#ifndef _ccms_crc32_hpp_
#define _ccms_crc32_hpp_

#include <stddef.h>

namespace ccms
{
	boost::uint32_t Crc32(const char * buf, size_t len);

	struct CRC32_CTX
	{
		boost::uint32_t _state;
	};

	int CRC32_Init(CRC32_CTX *c);
	int CRC32_Update(CRC32_CTX *c, const void *data, size_t len);
	int CRC32_Final(unsigned char *md, CRC32_CTX *c);

}

#endif

