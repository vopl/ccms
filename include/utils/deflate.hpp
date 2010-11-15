#ifndef _ccms_deflate_h_
#define _ccms_deflate_h_

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	bool stringDeflate(std::string &data, int level);
	
	//////////////////////////////////////////////////////////////////////////
	bool stringGzip(std::string &data, int level);

	//////////////////////////////////////////////////////////////////////////
	class CompressorStream
	{
	public:
		virtual ~CompressorStream(){};

		virtual char *push(size_t inSize, bool finish) = 0;
		virtual size_t inProcessed() = 0;
		virtual size_t inAvail() = 0;
		virtual bool compress(const char *out, size_t &outSize, bool &finish) = 0;
	};
	typedef boost::shared_ptr<CompressorStream> CompressorStreamPtr;

	//////////////////////////////////////////////////////////////////////////
	class CompressorStreamDeflate
		: public CompressorStream
	{
		void *_impl;
	public:
		CompressorStreamDeflate(int level);
		~CompressorStreamDeflate();
		virtual char *push(size_t inSize, bool finish);
		virtual size_t inProcessed();
		virtual size_t inAvail();
		virtual bool compress(const char *out, size_t &outSize, bool &finish);
	};

	//////////////////////////////////////////////////////////////////////////
	class CompressorStreamGzip
		: public CompressorStream
	{
		void *_impl;
	public:
		CompressorStreamGzip(int level);
		~CompressorStreamGzip();
		virtual char *push(size_t inSize, bool finish);
		virtual size_t inProcessed();
		virtual size_t inAvail();
		virtual bool compress(const char *out, size_t &outSize, bool &finish);
	};

	
}

#endif
