#include "stdafx.h"
#include "utils/deflate.hpp"
#include <zlib.h>

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	bool deflateString(std::string &data, int level)
	{
		if(data.empty()) return false;

		z_stream strm = {};
		deflateInit(&strm, level);

		strm.next_in = (Bytef *)data.data();
		strm.avail_in = data.size();

		std::vector<char> outBuf(strm.avail_in);

		while(strm.avail_in)
		{
			strm.avail_out = outBuf.size() - strm.total_out;
			if(strm.avail_out < 32)
			{
				outBuf.resize(outBuf.size()*10/8 + 1024);
				strm.avail_out = outBuf.size() - strm.total_out;
			}

			strm.next_out = (Bytef *)&outBuf[strm.total_out];
			int dres = deflate(&strm, Z_FINISH);
			assert(Z_STREAM_ERROR != dres);
		}
		data.assign(outBuf.begin(), outBuf.begin()+strm.total_out);
		deflateEnd(&strm);

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool gzipString(std::string &data, int level)
	{
		if(data.empty()) return false;

		z_stream strm = {};

		deflateInit2(&strm, level, Z_DEFLATED, 15+16, 8, Z_DEFAULT_STRATEGY);

		strm.next_in = (Bytef *)data.data();
		strm.avail_in = data.size();

		std::vector<char> outBuf(strm.avail_in);

		while(strm.avail_in)
		{
			strm.avail_out = outBuf.size() - strm.total_out;
			if(strm.avail_out < 32)
			{
				outBuf.resize(outBuf.size()*10/8 + 1024);
				strm.avail_out = outBuf.size() - strm.total_out;
			}

			strm.next_out = (Bytef *)&outBuf[strm.total_out];
			int dres = deflate(&strm, Z_FINISH);
			assert(Z_STREAM_ERROR != dres);
		}
		data.assign(outBuf.begin(), outBuf.begin()+strm.total_out);
		deflateEnd(&strm);

		return true;
	}
}
