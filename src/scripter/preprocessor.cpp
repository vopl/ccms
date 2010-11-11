#include "stdafx.h"
#include "scripter/preprocessor.hpp"
#include "utils/utf8.h"

namespace ccms
{
	using namespace boost::spirit::classic;

	//////////////////////////////////////////////////////////////////////////
	void Preprocessor::onCode(const char *first, const char *last)
	{
		size_t size = last - first;
		if(size >= _oc.length())
		{
			if(!_oc.compare(0,_oc.length(),first,_oc.length()))
			{
				size -= _oc.length();
				first += _oc.length();
			}
		}
		if(size >= _cc.length())
		{
			if(!_cc.compare(0,_cc.length(),last-_cc.length(), _oc.length()))
			{
				size -= _cc.length();
				last -= _cc.length();
			}
		}
		_result += lineFor(first);
		_result.append(first, last);
		_result +="\n";
	}

	//////////////////////////////////////////////////////////////////////////
	void Preprocessor::onCodePrint(const char *first, const char *last)
	{
		size_t size = last - first;
		if(size >= _ocPrint.length())
		{
			if(!_ocPrint.compare(0,_ocPrint.length(),first,_ocPrint.length()))
			{
				size -= _ocPrint.length();
				first += _ocPrint.length();
			}
		}
		if(size >= _cc.length())
		{
			if(!_cc.compare(0,_cc.length(),last-_cc.length(), _oc.length()))
			{
				size -= _cc.length();
				last -= _cc.length();
			}
		}
		_result += lineFor(first);
		_result += "print(";
		_result.append(first, last);
		_result +=")\n";
	}

	//////////////////////////////////////////////////////////////////////////
	void Preprocessor::onText(const char *first, const char *last)
	{
		_result += "print(\"";

		//далее переработаный utf8::utf16to8, игнорирование невалидных символов
		while (first != last)
		{
			utf8::uint32_t cp;
			utf8::internal::utf_error err = utf8::internal::validate_next(first, last, &cp);
			if(utf8::internal::UTF8_OK != err)
			{
				first++;
				continue;
			}

			char tmp[32];
			if (cp > 0xffff)
			{
				unsigned c1 = static_cast<unsigned>((cp >> 10)   + utf8::internal::LEAD_OFFSET);
				unsigned c2 = static_cast<unsigned>((cp & 0x3ff) + utf8::internal::TRAIL_SURROGATE_MIN);
				sprintf(tmp, "\\u%04x\\u%04x", c1, c2);
			}
			else
			{
				sprintf(tmp, "\\u%04x", cp);
			}
			_result += tmp;
		}

		_result +="\");\n";
	}

	//////////////////////////////////////////////////////////////////////////
	std::string Preprocessor::lineFor(const char *pos)
	{
		assert(pos<=_source->data()+_source->length());
		assert(pos>=_source->data());
		size_t lineNum = 1;
		const char *src = _source->data();
		for(; src<pos; src++)
		{
			if(*src == '\n')
			{
				lineNum++;
			}
		}

		char num[32];
		sprintf(num, "%u", lineNum);

		std::string res = "//@line ";
		res += num;
		res += _name;
		res += "\n";
		return res;

	}

	//////////////////////////////////////////////////////////////////////////
	Preprocessor::Preprocessor()
		: _source(NULL)
	{
		r_comment = comment_p("//") | comment_p("/*", "*/");
		r_string = confix_p('"', *c_escape_ch_p, '"') | confix_p('\'', *c_escape_ch_p, '\'');

		r_codeElement = r_string|r_comment|anychar_p;

		r_code1 = confix_p(r_oc, (*r_codeElement), r_cc);
		r_code2 = confix_p(r_oc, (*r_codeElement), "");

		r_codePrint1 = confix_p(r_ocPrint, (*r_codeElement), r_cc);
		r_codePrint2 = confix_p(r_ocPrint, (*r_codeElement), "");

		r_text = (+(anychar_p - r_oc));

		_rule = *(
			r_text[Functor(this, &Preprocessor::onText)]|
			(r_codePrint1|r_codePrint2)[Functor(this, &Preprocessor::onCodePrint)]|
			(r_code1|r_code2)[Functor(this, &Preprocessor::onCode)]
			);
	}

	//////////////////////////////////////////////////////////////////////////
	void Preprocessor::setup(const char *oc, const char *ocPrint, const char *cc)
	{
		_oc=oc;
		_ocPrint = ocPrint;
		_cc=cc;


		r_oc = strlit<std::string::const_iterator>(_oc.begin(), _oc.end());
		r_ocPrint = strlit<std::string::const_iterator>(_ocPrint.begin(), _ocPrint.end());
		r_cc = strlit<std::string::const_iterator>(_cc.begin(), _cc.end());
	}

	//////////////////////////////////////////////////////////////////////////
	bool Preprocessor::process(const std::string &src, const std::string &name, TUtf16data &res)
	{
		if(name.size()>=3 && name.find_last_of(".js", name.size()-3) == name.size()-3)
		{
			res.reserve(src.size()*2);

			const char *begin = src.data();
			const char *end = src.data()+src.size();
			if(utf8::starts_with_bom(begin, end))
			{
				//drop bom
				begin += 3;
			}

			try
			{
				utf8::utf8to16(begin, end, std::back_insert_iterator<std::vector<utf8::uint16_t> >(res), false);
			}
			catch(...)
			{
				return false;
			}
			return true;
		}
		size_t approxOutSize = src.size()*10 + 200;
		_source = &src;
		_name = name;
		if(!_name.empty())
		{
			_name = " \""+_name+"\"";
		}
		if(_result.capacity() < approxOutSize)
		{
			_result.reserve(approxOutSize);
		}
		_result = "//@line 1";
		_result += _name;
		_result += "\n";

		const char *begin = src.data();
		const char *end = src.data()+src.size();
		if(utf8::starts_with_bom(begin, end))
		{
			//drop bom
			begin += 3;
		}
		parse(begin, end, _rule, nothing_p);
		_result += lineFor(end);

		res.clear();
		if(res.capacity() < approxOutSize)
		{
			res.reserve(approxOutSize);
		}

		try
		{
			utf8::utf8to16(_result.begin(), _result.end(), std::back_insert_iterator<std::vector<utf8::uint16_t> >(res), false);
			std::string().swap(_result);
		}
		catch(...)
		{
			return false;
		}
		return true;

	}

}
