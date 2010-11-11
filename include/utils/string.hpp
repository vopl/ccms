#ifndef _ccms_utils_string_h_
#define _ccms_utils_string_h_

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	// в s1 ищет delim, заполняет его нулями и возвращает s1 после delim
	inline 
		char *ss_split(char *s1, const char *delim)
	{
		char *s2 = strstr(s1, delim);
		if(s2)
		{
			s2[0] = 0;
			size_t dlen = strlen(delim);
			s2[dlen-1] = 0;
			s2 += dlen;
		}
		return s2;
	}

	//////////////////////////////////////////////////////////////////////////
	//сравнение без регистра
	inline
	bool ss_eq_nc(const std::string& s1, const std::string& s2)
	{
		std::string::const_iterator p1 = s1.begin();
		std::string::const_iterator p2 = s2.begin();
		std::string::const_iterator l1 = s1.end();
		std::string::const_iterator l2 = s2.end();

		while(p1 != l1 && p2 != l2) {
			if(std::toupper(*(p1++)) != std::toupper(*(p2++)))
				return false;
		}

		return (s2.size() == s1.size()) ? true : false;
	}

	//////////////////////////////////////////////////////////////////////////
	//сравнение без регистра
	inline
	bool ss_eq_nc(const std::string& s1, const std::string& s2, size_t n)
	{
		std::string::const_iterator p1 = s1.begin();
		std::string::const_iterator p2 = s2.begin();
		bool good = (n <= s1.length() && n <= s2.length());
		std::string::const_iterator l1 = good ? (s1.begin() + n) : s1.end();
		std::string::const_iterator l2 = good ? (s2.begin() + n) : s2.end();

		while(p1 != l1 && p2 != l2) {
			if(std::toupper(*(p1++)) != std::toupper(*(p2++)))
				return false;
		}

		return good;
	}

	//////////////////////////////////////////////////////////////////////////
	//сравнение без регистра
	inline
		bool ss_eq_nc(const char *s1, const char *s2)
	{
		while(*s1 && *s2) {
			if(std::toupper(*(s1++)) != std::toupper(*(s2++)))
				return false;
		}

		return *s2 == *s1;
	}

	//////////////////////////////////////////////////////////////////////////
	inline
		char hexToChar(char first, char second)
	{
		int digit;

		digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0'));
		digit *= 16;
		digit += (second >= 'A' ? ((second & 0xDF) - 'A') + 10 : (second - '0'));
		return static_cast<char>(digit);
	}

	//////////////////////////////////////////////////////////////////////////
	inline
	std::string urldecode(const std::string& src)
	{
		std::string result;
		std::string::const_iterator iter;
		char c;

		for(iter = src.begin(); iter != src.end(); ++iter)
		{
			switch(*iter)
			{
			case '+':
				result.append(1, ' ');
				break;
			case '%':
				if(std::distance(iter, src.end()) >= 2
					&& std::isxdigit(*(iter + 1)) && std::isxdigit(*(iter + 2)))
				{
					c = *++iter;
					result.append(1, hexToChar(c, *++iter));
				}
				else
				{
					result.append(1, '%');
				}
				break;

			default:
				result.append(1, *iter);
				break;
			}
		}

		return result;
	}

	//////////////////////////////////////////////////////////////////////////
	inline
		std::string hexdecode(const std::string& src)
	{
		std::string result;
		std::string::const_iterator iter;
		char c;

		for(iter = src.begin(); iter != src.end(); ++iter)
		{
			switch(*iter)
			{
			case '%':
				if(std::distance(iter, src.end()) >= 2
					&& std::isxdigit(*(iter + 1)) && std::isxdigit(*(iter + 2)))
				{
					c = *++iter;
					result.append(1, hexToChar(c, *++iter));
				}
				else
				{
					result.append(1, '%');
				}
				break;

			default:
				result.append(1, *iter);
				break;
			}
		}

		return result;
	}

	//////////////////////////////////////////////////////////////////////////
	inline std::string &trim_quotes(std::string &s)
	{
		if(s.length()<2)
		{
			return s;
		}

		char c1 = s[0];
		char c2 = s[s.size()-1];
		if(	c1 == '\'' && c2 == '\'' ||
			c1 == '"'  && c2 == '"' )
		{
			s.erase(s.begin());
			s.erase(s.end()-1);
		}
		return s;
	}

	//////////////////////////////////////////////////////////////////////////
	inline 
		std::string extractBetween(const std::string& data, 
		const std::string& separator1, 
		const std::string& separator2)
	{
		std::string result;
		std::string::size_type start, limit;

		start = data.find(separator1, 0);
		if(std::string::npos != start)
		{
			start += separator1.length();
			limit = data.find(separator2, start);
			if(std::string::npos != limit)
				result = data.substr(start, limit - start);
		}

		return result;
	}

	//////////////////////////////////////////////////////////////////////////
	inline
		size_t trim(std::string &str)
	{
		while(str.size() && isspace((unsigned char)str[str.size()-1]))
		{
			str.erase(str.size()-1);
		}
		while(str.size() && isspace((unsigned char)str[0]))
		{
			str.erase(0, 1);
		}
		return str.length();
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Iterator>
		void tolowerLatin(Iterator begin, Iterator end)
	{
		for(; begin!=end; begin++)
		{
			char &c = *begin;
			if(c>='A' && c<='Z')
			{
				c = c - 'A' + 'a';
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	inline
		std::string & tolowerLatin(std::string &str)
	{
		tolowerLatin(str.begin(), str.end());
		return str;
	}

}
#endif
