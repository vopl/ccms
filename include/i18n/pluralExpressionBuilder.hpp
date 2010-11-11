#ifndef _ccms_pluralExpressionBuilder_hpp_
#define _ccms_pluralExpressionBuilder_hpp_

#include "i18n/pluralExpression.hpp"

namespace ccms{namespace plural
{
	class ExpressionBuilder
	{
		struct ActionS
		{
			ExpressionBuilder &_eb;
			EActionType _eat;
			ActionS(ExpressionBuilder &eb, EActionType eat)
				: _eb(eb)
				, _eat(eat)
			{
			}
			void operator()(const char *beg, const char *end)const
			{
				_eb.action(_eat, 1);
			}
			void operator()(unsigned long int num)const
			{
				_eb.action(_eat, num);
			}
		};

		std::vector<Expression *> _expressions;
		void action(EActionType eat, unsigned long int num)
		{
			Expression *newe = new Expression();
			newe->_eat = eat;

			switch(eat)
			{
			case eat_num:
				newe->_val._num = num;
				_expressions.push_back(newe);
				return;
			case eat_var:
				_expressions.push_back(newe);
				return;
			case eat_null:
				assert(0);
				delete newe;
				return;
			}

			switch(eat & 0xf00)
			{
			case 0x100:
				newe->_val._args[0] = _expressions[_expressions.size()-1];
				_expressions.pop_back();
				_expressions.push_back(newe);
				return;
			case 0x200:
				newe->_val._args[0] = _expressions[_expressions.size()-2];
				newe->_val._args[1] = _expressions[_expressions.size()-1];
				_expressions.pop_back();
				_expressions.pop_back();
				_expressions.push_back(newe);
				return;
			case 0x300:
				newe->_val._args[0] = _expressions[_expressions.size()-3];
				newe->_val._args[1] = _expressions[_expressions.size()-2];
				newe->_val._args[2] = _expressions[_expressions.size()-1];
				_expressions.pop_back();
				_expressions.pop_back();
				_expressions.pop_back();
				_expressions.push_back(newe);
				return;
			}
			assert(0);
		}

		ActionS mka(EActionType eat)
		{
			return ActionS(*this, eat);
		}


	public:
		ExpressionBuilder()
		{
		}
		~ExpressionBuilder()
		{
			BOOST_FOREACH(Expression *e, _expressions)
			{
				delete e;
			}
		}

		ExpressionPtr parse(const char *beg, const char *end)
		{
			using namespace boost::spirit::classic;
			rule<> expression;

			rule<> num = uint_p[mka(eat_num)];
			rule<> var = str_p("n")[mka(eat_var)];

			rule<> expression0 = num|var;
			rule<> expression1 = (expression0 | (ch_p('(') >> expression >> ')'));

			rule<> expression2 = expression1 >> repeat_p(0,1)[
				ch_p('!') >> expression[mka(eat_operNot)]
			];
			rule<> expression3 = expression2 >> *(
				ch_p('*') >> expression[mka(eat_operMul)]|
				ch_p('/') >> expression[mka(eat_operDiv)]|
				ch_p('%') >> expression[mka(eat_operMod)]
			);
			rule<> expression4 = expression3 >> *(
				ch_p('+') >> expression[mka(eat_operAdd)]|
				ch_p('-') >> expression[mka(eat_operSub)]
			);
			rule<> expression5 = expression4 >> *(
				str_p(">=") >> expression[mka(eat_operGteq)]|
				str_p("<=") >> expression[mka(eat_operLteq)]|
				ch_p('>') >> expression[mka(eat_operGt)]|
				ch_p('<') >> expression[mka(eat_operLt)]
			);
			rule<> expression6 = expression5 >> *(
				str_p("==") >> expression[mka(eat_operEq)]|
				str_p("!=") >> expression[mka(eat_operNe)]
			);
			rule<> expression7 = expression6 >> *(
				str_p("&&") >> expression[mka(eat_operAnd)]
			);
			rule<> expression8 = expression7 >> *(
				str_p("||") >> expression[mka(eat_operOr)]
			);
			rule<> expression9 = expression8 >> repeat_p(0,1)[
				ch_p('?') >> expression >> ch_p(':') >> expression[mka(eat_operQ)]
			];
			expression = expression9;

			if(boost::spirit::classic::parse(beg, end, expression, space_p).full)
			{
				assert(1 == _expressions.size());
				ExpressionPtr res(_expressions[0]);
				_expressions.clear();
				return res;
			}
			return ExpressionPtr();
		}

	};
}}
#endif
