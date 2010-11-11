#ifndef _ccms_preprocessor_hpp_
#define _ccms_preprocessor_hpp_

#include "utils/utf8.h"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	typedef std::vector<utf8::uint16_t> TUtf16data;

	//////////////////////////////////////////////////////////////////////////
	class Preprocessor
	{
		std::string _oc, _ocPrint,_cc;

		boost::spirit::classic::rule<> r_oc;
		boost::spirit::classic::rule<> r_ocPrint;
		boost::spirit::classic::rule<> r_cc;
		boost::spirit::classic::rule<> r_comment;
		boost::spirit::classic::rule<> r_string;

		boost::spirit::classic::rule<> r_codeElement;

		boost::spirit::classic::rule<> r_code1;
		boost::spirit::classic::rule<> r_code2;

		boost::spirit::classic::rule<> r_codePrint1;
		boost::spirit::classic::rule<> r_codePrint2;

		boost::spirit::classic::rule<> r_text;
		boost::spirit::classic::rule<> _rule;


		typedef void (Preprocessor:: *TActor)(const char *begin, const char *end);
		void onCode(const char *begin, const char *end);
		void onCodePrint(const char *begin, const char *end);
		void onText(const char *begin, const char *end);
		struct Functor
		{
			Preprocessor *_p;
			TActor _actor;
			Functor(Preprocessor *p, TActor actor)
				: _p(p)
				, _actor(actor)
			{
			}

			void operator()(const char *begin, const char *end) const
			{
				return (_p->*_actor)(begin, end);
			}
		};

		const std::string *_source;
		std::string _result;
		std::string _name;

		std::string lineFor(const char *pos);
	public:
		Preprocessor();
		void setup(const char *oc, const char *ocPrint, const char *cc);

		bool process(const std::string &src, const std::string &name, TUtf16data &res);
	};
}
#endif
