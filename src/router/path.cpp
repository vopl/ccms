#include "stdafx.h"
#include "router/path.hpp"
#include "utils/utf8.h"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Path::Path(std::deque<std::string>::const_iterator begin, std::deque<std::string>::const_iterator end)
		: std::deque<std::string>(begin, end)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Path::Path()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Path::Path(const Path &from)
		: std::deque<std::string>(from)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Path::Path(const char *from)
	{
		*this = from;
	}

	//////////////////////////////////////////////////////////////////////////
	Path::Path(const std::string &from)
	{
		*this = from;
	}

	//////////////////////////////////////////////////////////////////////////
	Path::~Path()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Path &Path::operator=(const Path &from)
	{
		(std::deque<std::string> &)*this = from;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	Path &Path::operator=(const char *from)
	{
		clear();
		if(!from || !from[0])
		{
			return *this;
		}

		const char *next = strchr(from, '/');
		while(next)
		{
			if(next != from)
			{
				push_back(std::string(from, next));
			}
			from = next+1;
			next = strchr(from, '/');
		}
		if(from && from[0])
		{
			push_back(std::string(from));
		}

		normalize();
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	Path &Path::operator=(const std::string &from)
	{
		return *this = from.c_str();
	}

	//////////////////////////////////////////////////////////////////////////
	Path &Path::operator/=(const Path &from)
	{
		insert(end(), from.begin(), from.end());
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	Path &Path::operator/=(const char *from)
	{
		return *this /= Path(from);
	}

	//////////////////////////////////////////////////////////////////////////
	Path &Path::operator/=(const std::string &from)
	{
		return *this /= Path(from);
	}


	//////////////////////////////////////////////////////////////////////////
	Path Path::operator/(const Path &from)const
	{
		return Path(*this)/=from;
	}

	//////////////////////////////////////////////////////////////////////////
	Path Path::operator/(const char *from)const
	{
		return Path(*this)/=from;
	}

	//////////////////////////////////////////////////////////////////////////
	Path Path::operator/(const std::string &from)const
	{
		return Path(*this)/=from;
	}

	//////////////////////////////////////////////////////////////////////////
	Path Path::beforeBack() const
	{
		if(size()<2)
		{
			return Path();
		}

		return Path(begin(), --end());
	}

	//////////////////////////////////////////////////////////////////////////
	Path Path::afterFront() const
	{
		if(size()<2)
		{
			return Path();
		}

		return Path(++begin(), end());
	}

	//////////////////////////////////////////////////////////////////////////
	std::string Path::string() const
	{
		std::string res;
		for(size_t i(0); i<size(); i++)
		{
			res += operator[](i) + '/';
		}
		if(!res.empty())
		{
			res.erase(--res.end());
		}

		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	Path::operator std::string()const
	{
		return string();
	}


	//////////////////////////////////////////////////////////////////////////
	void Path::normalize()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Path::eqSmaller(const Path &with) const
	{
		return std::equal(begin(), begin()+std::min(size(), with.size()), with.begin());
	}

	//////////////////////////////////////////////////////////////////////////
	bool Path::isValidPathPart(const std::string &part)
	{
		if(part.empty())
		{
			return false;
		}

		std::vector<utf8::uint32_t> utf32;
		try
		{
			utf8::utf8to32(part.begin(), part.end(), std::back_insert_iterator<std::vector<utf8::uint32_t> >(utf32));
		}
		catch(...)
		{
			return false;
		}

		BOOST_FOREACH(utf8::uint32_t &ch, utf32)
		{
			if(ch < 32)
			{
				return false;
			}
			switch(ch)
			{
			case '"':
			case '\'':
			case '<':
			case '>':
			case '|':
			case ':':
			case '\\':
			case '/':
			case '?':
			case '*':
				return false;
			}
		}

		return true;
	}

}
