#ifndef _ccms_mime_utils_hpp_
#define _ccms_mime_utils_hpp_

#include "utils/base64.h"
#include "utils/ncvt.h"

namespace ccms{namespace mime{
	inline 
		void encodeNonAscii(std::string &str, bool quotesMode = false)
	{
		bool needEncode = false;
		for(size_t i(0); i<str.size(); i++)
		{
			if(((unsigned char)str[i]) > 127)
			{
				needEncode = true;
				break;
			}
		}

		if(needEncode)
		{
			str = "=?UTF-8?B?" + base64_encode(str) + "?=";
		}
		else
		{
			if(quotesMode)
			{
				size_t pos=0;
				while(std::string::npos != (pos = str.find('"', pos)))
				{
					str.insert(pos, 1, '\\');
					pos+=2;
				}
				str.insert(0, 1, '"');
				str += '"';
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	inline 
		std::string genBoundary()
	{
		std::string res;
		res += "_";
		res += _ntoa(rand());
		res += _ntoa(rand());
		res += _ntoa(rand());
		res += _ntoa(rand());
		res += _ntoa(rand());
		res += _ntoa(rand());
		res += _ntoa(rand());
		res += _ntoa(rand());
		res += "_";
		return res;
	}
}}
#endif
