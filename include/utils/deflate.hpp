#ifndef _ccms_deflate_h_
#define _ccms_deflate_h_

namespace ccms
{
	bool deflateString(std::string &data, int level);
	bool gzipString(std::string &data, int level);
}

#endif
