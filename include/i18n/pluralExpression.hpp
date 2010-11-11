#ifndef _ccms_pluralExpression_hpp_
#define _ccms_pluralExpression_hpp_

namespace ccms{namespace plural
{
	//используется при составлении граматики - метит действия, используется в выражении
	enum EActionType
	{
		eat_null = 0,

		//унарнные операторы
		eat_operNot		= 0x101,

		//бинарнные операторы
		eat_operMul		= 0x201,
		eat_operDiv		= 0x202,
		eat_operMod		= 0x203,
		eat_operAdd		= 0x204,
		eat_operSub		= 0x205,
		eat_operGteq	= 0x206,
		eat_operLteq	= 0x207,
		eat_operGt		= 0x208,
		eat_operLt		= 0x209,
		eat_operEq		= 0x20a,
		eat_operNe		= 0x20b,
		eat_operAnd		= 0x20c,
		eat_operOr		= 0x20d,

		//тринарнные операторы
		eat_operQ		= 0x301,

		//число
		eat_num	= 0x10001,

		//переменная
		eat_var	= 0x10002,
	};

	//выражение
	class Expression
	{
		friend class ExpressionBuilder;
		EActionType _eat;
		union
		{
			unsigned long int _num;
			Expression *_args[3];
		} _val;

		Expression()
			: _eat(eat_null)
			, _val()
		{

		}

	public:
		~Expression()
		{
			if(eat_num != _eat)
			{
				delete _val._args[0];
				delete _val._args[1];
				delete _val._args[2];
			}
		}

		unsigned long int eval(unsigned long int var)
		{
			switch(_eat)
			{
			case eat_operNot:
				return _val._args[0]->eval(var)?0:1;
			case eat_operMul:
				return _val._args[0]->eval(var) * _val._args[1]->eval(var);
			case eat_operDiv:
				return _val._args[0]->eval(var) / _val._args[1]->eval(var);
			case eat_operMod:
				return _val._args[0]->eval(var) % _val._args[1]->eval(var);
			case eat_operAdd:
				return _val._args[0]->eval(var) + _val._args[1]->eval(var);
			case eat_operSub:
				return _val._args[0]->eval(var) - _val._args[1]->eval(var);
			case eat_operGteq:
				return _val._args[0]->eval(var) >= _val._args[1]->eval(var);
			case eat_operLteq:
				return _val._args[0]->eval(var) <= _val._args[1]->eval(var);
			case eat_operGt:
				return _val._args[0]->eval(var) > _val._args[1]->eval(var);
			case eat_operLt:
				return _val._args[0]->eval(var) < _val._args[1]->eval(var);
			case eat_operEq:
				return _val._args[0]->eval(var) == _val._args[1]->eval(var);
			case eat_operNe:
				return _val._args[0]->eval(var) != _val._args[1]->eval(var);
			case eat_operAnd:
				return (_val._args[0]->eval(var) && _val._args[1]->eval(var))?1:0;
			case eat_operOr:
				return (_val._args[0]->eval(var) || _val._args[1]->eval(var))?1:0;
			case eat_operQ:
				return _val._args[0]->eval(var) ? _val._args[1]->eval(var) : _val._args[2]->eval(var);
			case eat_num:
				return _val._num;
			case eat_var:
				return var;
			}

			assert(0);
			return (unsigned long int)-1;
		}
	};
	typedef boost::shared_ptr<Expression> ExpressionPtr;


}}
#endif
