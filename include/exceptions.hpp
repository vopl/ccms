#ifndef _ccms_exceptions_h_
#define _ccms_exceptions_h_

#include <stdexcept>

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class exception
		: public std::exception
	{
		std::string _what;
	public:
		exception()  throw()
		{
		}

		exception(const exception &what) throw()
			: _what(what.what())
		{
		}

		exception(const char *what) throw()
			: _what(what)
		{
		}

		exception(const std::string &what) throw()
			: _what(what)
		{
		}
		
		virtual ~exception() throw()
		{
		}

		virtual const char* what() const throw() { return _what.c_str(); }
	};

	//////////////////////////////////////////////////////////////////////////
	class exception_noFile
		: public exception
	{
		std::string _what;
	public:
		exception_noFile(const char *file, const char *from=NULL) throw()
			: _what(std::string("not found ") + (file?file:"<unknown>") + (from?(std::string(" from ")+from):""))
		{
		}
		virtual ~exception_noFile() throw()
		{
		}

		virtual const char* what() const throw() { return _what.c_str(); }

	};

	//////////////////////////////////////////////////////////////////////////
	class exception_badPathPart
		: public exception
	{
		std::string _what;
	public:
		exception_badPathPart(const char *part) throw()
			: _what(std::string("bad path part: ") + part)
		{
		}

		~exception_badPathPart() throw()
		{
		}

		virtual const char* what() const throw() { return _what.c_str(); }

	};


}
#endif
