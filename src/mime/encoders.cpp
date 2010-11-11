#include "stdafx.h"
#include "mime/encoders.hpp"
#include "router/execContext.hpp"
#include "utils/base64.h"

namespace ccms{namespace mime{

	//////////////////////////////////////////////////////////////////////////
	bool encode_7bit(jsval &val, const char *data, size_t len)
	{
		//ничего
		//1000 надо контролировать

		if(data)
		{
			val = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, data, len));
			return true;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool encode_8bit(jsval &val, const char *data, size_t len)
	{
		//ничего
		//1000 надо контролировать

		if(data)
		{
			val = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, data, len));
			return true;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool encode_qp(jsval &val, const char *data, size_t len)
	{
		static char digits16[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		const char *iter, *end;

		if(data)
		{
			iter = data;
			end = data+len;
		}
		else
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &val, "s", &iter)) return false;
			end = iter + strlen(iter);
		}

		std::string res;
		size_t lineLen=0;
		for(; iter!=end; iter++)
		{
			//printable 33-60, 62-126
			if(*iter >= 33 && *iter <= 126 && *iter != 61)
			{
				if(lineLen >= 76)
				{
					res += "=\r\n";
					lineLen = 0;
				}
				res += *iter;
				lineLen++;
			}
			//9, 32 не в конце линии
			else if((*iter == 9 || *iter == 32) && iter!=end-1 && iter[1]!='\r' && iter[1]!='\n')
			{
				if(lineLen >= 76)
				{
					res += "=\r\n";
					lineLen = 0;
				}
				res += *iter;
				lineLen++;
			}
			//\r\n \r \n => CRLF
			else if(*iter=='\r')
			{
				if(iter!=end-1 && iter[1]=='\n')
				{
					iter++;
				}

				if(lineLen >= 73)
				{
					res += "=\r\n";
					lineLen = 0;
				}
				res += "=0D";
				lineLen+=3;

				if(lineLen >= 73)
				{
					res += "=\r\n";
					lineLen = 0;
				}
				res += "=0A";
				lineLen+=3;
			}
			else if(*iter=='\n')
			{
				if(lineLen >= 73)
				{
					res += "=\r\n";
					lineLen = 0;
				}

				res += "=0D";
				lineLen+=3;

				if(lineLen >= 73)
				{
					res += "=\r\n";
					lineLen = 0;
				}

				res += "=0A";
				lineLen+=3;
			}
			else
			{
				if(lineLen >= 73)
				{
					res += "=\r\n";
					lineLen = 0;
				}

				res += "=XX";
				res[res.size()-2] = digits16[((*iter)>>4)&0x0f];
				res[res.size()-1] = digits16[((*iter))&0x0f];
				lineLen+=3;
			}

		}

		val = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, res.data(), res.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool encode_base64(jsval &val, const char *data, size_t len)
	{
		std::string res;
		if(data)
		{
			res = base64_encode(data, len, 76);
		}
		else
		{
			char *iter;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &val, "s", &iter)) return false;
			res = base64_encode(iter, strlen(iter), 76);
		}


		val = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, res.data(), res.size()));
		return true;
	}

}}
