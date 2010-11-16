#include "stdafx.h"
#include "utils/deflate.hpp"
#include <zlib.h>

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	bool stringZlib(std::string &data, int level, bool isGzip)
	{
		if(data.empty()) return false;

		z_stream strm = {};
		gz_header hdr = {};

		if(isGzip)
		{
			deflateInit2(&strm, level, Z_DEFLATED, 15+16, 8, Z_DEFAULT_STRATEGY);

 			hdr.os = 255;
			hdr.hcrc = true;
			deflateSetHeader(&strm, &hdr);

		}
		else
		{
			deflateInit(&strm, level);
		}



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
	bool stringDeflate(std::string &data, int level)
	{
		return stringZlib(data, level, false);
	}


	//////////////////////////////////////////////////////////////////////////
	bool stringGzip(std::string &data, int level)
	{
		return stringZlib(data, level, true);
	}


	//////////////////////////////////////////////////////////////////////////
	class CompressorZlib
	{
		std::vector<char> _buf;
		bool _finish;
		z_stream _strm;
		gz_header _hdr;
	public:
		CompressorZlib(int level, bool isGzip)
			: _strm()
			, _hdr()
			, _finish(false)
		{
			if(isGzip)
			{
				deflateInit2(&_strm, level, Z_DEFLATED, 15+16, 8, Z_DEFAULT_STRATEGY);

				_hdr.os = 255;
				_hdr.hcrc = true;
				deflateSetHeader(&_strm, &_hdr);

			}
			else
			{
				deflateInit(&_strm, level);
			}
		}

		~CompressorZlib()
		{
			deflateEnd(&_strm);
		}

		char *push(size_t inSize, bool finish)
		{
			size_t oldSize = _buf.size();
			_buf.resize(oldSize+inSize);
			_finish = finish;

			if(_buf.empty())
			{
				assert(0);
				return NULL;
			}

			return &_buf[oldSize];
		}

		size_t inProcessed()
		{
			return _strm.total_in;
		}

		size_t inAvail()
		{
			return _buf.size();
		}

		bool compress(const char *out, size_t &outSize, bool &finish)
		{
			if(_buf.empty())
			{
				_strm.next_in = NULL;
				_strm.avail_in = 0;
			}
			else
			{
				_strm.next_in = (Bytef *)&_buf[0];
				_strm.avail_in = _buf.size();
			}

			std::size_t inAmount = _strm.total_in;
			std::size_t outAmount = _strm.total_out;

			_strm.next_out = (Bytef *)out;
			_strm.avail_out = outSize;

			int dres = deflate(&_strm, _finish?Z_FINISH:Z_SYNC_FLUSH);
			assert(Z_STREAM_ERROR != dres);

			inAmount = _strm.total_in - inAmount;
			if(inAmount)
			{
				_buf.erase(_buf.begin(), _buf.begin()+inAmount);
			}

			outAmount = _strm.total_out - outAmount;
			outSize = outAmount;

			if(_finish && !_strm.avail_in)
			{
				finish = true;
			}
			else
			{
				finish = false;
			}

			return Z_STREAM_ERROR != dres;
		}
	};



	//////////////////////////////////////////////////////////////////////////
	CompressorStreamDeflate::CompressorStreamDeflate(int level)
		: _impl(new CompressorZlib(level, false))
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CompressorStreamDeflate::~CompressorStreamDeflate()
	{
		delete (CompressorZlib *)_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	char *CompressorStreamDeflate::push(size_t inSize, bool finish)
	{
		return ((CompressorZlib *)_impl)->push(inSize, finish);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CompressorStreamDeflate::inProcessed()
	{
		return ((CompressorZlib *)_impl)->inProcessed();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CompressorStreamDeflate::inAvail()
	{
		return ((CompressorZlib *)_impl)->inAvail();
	}

	//////////////////////////////////////////////////////////////////////////
	bool CompressorStreamDeflate::compress(const char *out, size_t &outSize, bool &finish)
	{
		return ((CompressorZlib *)_impl)->compress(out, outSize, finish);
	}

	//////////////////////////////////////////////////////////////////////////
	CompressorStreamGzip::CompressorStreamGzip(int level)
		: _impl(new CompressorZlib(level, true))
	{
	}

	//////////////////////////////////////////////////////////////////////////
	CompressorStreamGzip::~CompressorStreamGzip()
	{
		delete (CompressorZlib *)_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	char *CompressorStreamGzip::push(size_t inSize, bool finish)
	{
		return ((CompressorZlib *)_impl)->push(inSize, finish);
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CompressorStreamGzip::inProcessed()
	{
		return ((CompressorZlib *)_impl)->inProcessed();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t CompressorStreamGzip::inAvail()
	{
		return ((CompressorZlib *)_impl)->inAvail();
	}

	//////////////////////////////////////////////////////////////////////////
	bool CompressorStreamGzip::compress(const char *out, size_t &outSize, bool &finish)
	{
		return ((CompressorZlib *)_impl)->compress(out, outSize, finish);
	}

}
