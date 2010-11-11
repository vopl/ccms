#include "stdafx.h"
#include "i18n/numberFormatter.hpp"
#include "utils/ncvt.h"
#include "utils/dcvt.h"

namespace ccms
{
	const char NumberFormatter::_abc[37] =	"0123456789abcdefghijklmnopqrstuvwxyz";
	const char NumberFormatter::_ABC[37] =	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	//////////////////////////////////////////////////////////////////////////
	NumberFormatter::NumberFormatter()
		: _delim0('.')
		, _delim3('`')
		, _zero0(true)
		, _nan("NaN")
		, _inf("Infinity")
	{
		//%[флаги][ширина][.[флаги2]точность]тип[система—числени€]
		/*
			флаги
				-			заполн€ть влево
				0			чем заполн€ть влево, если не задано то пробел
				+			дл€ положительных выводить +
				пробел		дл€ положительных выводить пробел вместо +

			ширина
				целое положительное, количество символов в результате

			флаги2
				-			не заполн€ть вправо нул€ми, если не указано то заполн€етс€ нул€ми до точности

			точность
				целое положительное, количество символов после дес€тичной точки

			тип
				i
				f
				e E
				g G
				md - сокращать разр€ды в пользу приставок SI, использовать f
					http://ru.wikipedia.org/wiki/ѕриставки_—»
				mb - сокращать разр€ды в пользу двоичных приставок
					http://ru.wikipedia.org/wiki/ƒвоичные_приставки

			система—числени€
				N[U]
					N от 2 до 36
					U если есть то алфавит цифр в верхнем регистре

			из локали берутс€
				символ - разделитель целой и дробной части
				строка - разделитель 3 разр€дов
				буль - ведущий ноль в дробном<1
				строка - не число
				строка - бесконечность
				множители двоичные
				мнжители дес€тичные


		*/
		using namespace boost::xpressive;
		_re = 
			//флаги
			(s1=*(as_xpr('-')|'0'|'+'|' '))
			//ширина
			>> (s2=*_d)
			//флаги2 и точность
			>> (repeat<0,1>(as_xpr('.')
				>> (s3=repeat<0,1>(as_xpr('-')))
				>> (s4=*_d)	))
			//тип
			>> (s5= (as_xpr('i')|'f'|'e'|'g'|'E'|'G'|"md"|"mb"))
			//система—числени€
			>> ((s6= *_d) >> (s7=repeat<0,1>(as_xpr('U'))))
			;
	}

	//////////////////////////////////////////////////////////////////////////
	void NumberFormatter::setup(
		char delim0, 
		char delim3, 
		bool zero0, 
		const char *nan, 
		const char *inf,
		const TMFactors &factors10,
		const TMFactors &factors2)
	{
		_delim0 = delim0;
		_delim3 = delim3;
		_zero0 = zero0;
		_nan = nan?nan:"NaN";
		_inf = inf?inf:"Infinity";

		_factors10 = factors10;
		if(_factors10.end() == _factors10.find(0)) _factors10[0] = "";
		_factors2 = factors2;
		if(_factors2.end() == _factors2.find(0)) _factors2[0] = "";
	}

	//////////////////////////////////////////////////////////////////////////
	std::string NumberFormatter::format(double num, const char *format)
	{
		_res.clear();

		if(recognizeFormat(format))
		{
			if(_radix<2 || _radix>36)
			{
				_res = "[bad radix: ";
				_res += format;
				_res += "]";
				return _res;
			}
			if(_width>300 || _precision>300)
			{
				_res = "[bad width or precision: ";
				_res += format;
				_res += "]";
				return _res;
			}

			formatNumber_prepareFactor(num);

			//////////////////////////////////////////////////////////////////////////
			_digits.resize(64);
			int digitsAmount = _digits.size()-1;
			EDcvtRes dcvtRes = dcvt(num, _radix, &_digits[0], digitsAmount, _expR, _sign);
			switch(dcvtRes)
			{
			case edr_ok:
				break;
			case edr_nan:
				_res = _nan;
				return _res;
			case edr_inf:
				_res = _inf;
				return _res;
			case edr_den:
				_res = _nan;
				return _res;
			case edr_badRadix:
				assert(0);
				return _res;
			case edr_lowBuffer:
				assert(0);
				return _res;
			default:
				assert(0);
				return _res;
			}
			_digits.resize(digitsAmount);
			const char *abc = _radixUppercase?_ABC:_abc;
			for(int i(0); i<digitsAmount; i++)
			{
				_digits[i] = abc[_digits[i]];
			}

			formatNumber_intDigits();
			formatNumber_fracDigits();
			formatNumber_pads();
		}
		else
		{
			_res = "[bad format: ";
			_res += format;
			_res += "]";
		}


		return _res;
	}

	//////////////////////////////////////////////////////////////////////////
	bool NumberFormatter::recognizeFormat(const char *format)
	{
		if(boost::xpressive::regex_match(format, _what, _re))
		{
			_flags = ef_null;

			//////////////////////////////////////////////////////////////////////////
			const std::string &flags_s = _what[1];
			if(!flags_s.empty())
			{
				_flags &= ~ef_mask;
				for(size_t i(0); i<flags_s.size(); i++)
				{
					switch(flags_s[i])
					{
					case '-':
						_flags |= ef_alignLeft;
						break;
					case '0':
						_flags |= ef_padLeftZero;
						break;
					case '+':
						_flags |= ef_forcePositivePlus;
						break;
					case ' ':
						_flags |= ef_forcePositiveSpace;
						break;
					default:
						//ignore in release?
						assert(0);
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			const std::string &width_s = _what[2];
			if(!width_s.empty())
			{
				_width = _atoi(width_s.c_str());
			}
			else
			{
				_width = 0;
			}

			//////////////////////////////////////////////////////////////////////////
			const std::string &flags2_s = _what[3];
			if(!flags2_s.empty())
			{
				_flags &= ~ef2_mask;
				for(size_t i(0); i<flags2_s.size(); i++)
				{
					switch(flags2_s[i])
					{
					case '-':
						_flags |= ef2_noPadRightZero;
						break;
					default:
						//ignore in release?
						assert(0);
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			const std::string &precision_s = _what[4];
			if(!precision_s.empty())
			{
				_precision = _atoi(precision_s.c_str());
			}
			else
			{
				_precision = -1;
			}

			//////////////////////////////////////////////////////////////////////////
			const std::string &kind_s = _what[5];
			if(!kind_s.empty())
			{
				switch(kind_s[0])
				{
				case 'i':
					_kind = ek_i;
					break;
				case 'f':
					_kind = ek_f;
					break;
				case 'e':
					_kind = ek_e;
					break;
				case 'E':
					_kind = ek_E;
					break;
				case 'g':
					_kind = ek_g;
					break;
				case 'G':
					_kind = ek_G;
					break;
				case 'm':
					if(kind_s[1]=='d')
					{
						_kind = ek_md;
					}
					else
					{
						_kind = ek_mb;
					}
					break;
				}
			}
			else
			{
				_kind = ek_g;
			}

			//////////////////////////////////////////////////////////////////////////
			const std::string &radix_s = _what[6];
			if(!radix_s.empty())
			{
				_radix = _atoi(radix_s.c_str());
			}
			else
			{
				_radix = 10;
			}
			//////////////////////////////////////////////////////////////////////////
			const std::string &radixUppercase_s = _what[7];
			if(!radixUppercase_s.empty())
			{
				_radixUppercase = true;
			}
			else
			{
				_radixUppercase = false;
			}
			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool NumberFormatter::formatNumber_prepareFactor(double &num)
	{
		//g->f or e
		int expR = int(log(fabs(num))/log(double(_radix)) + (num>1?0.5:-0.5));
		if(ek_g == _kind)
		{
			if((expR>-7 && expR<12)) _kind = ek_f;
			else _kind = ek_e;
		}
		else if(ek_G == _kind)
		{
			if((expR>-7 && expR<12)) _kind = ek_f;
			else _kind = ek_E;
		}

		//наверное перенести до парса числа...
		switch(_kind)
		{
		case ek_e:
		case ek_E:
			if(num)
			{
				int order10 = int(log10(fabs(num)) + (num>1?0.5:-0.5));

				double mult = pow(10.0, order10);
				num /= mult;

				_factor = _kind==ek_e?'e':'E';
				if(order10>=0)
				{
					_factor += "+";
				}
				else
				{
					_factor += "-";
					order10 = -order10;
				}
				if(order10<10)
					_factor += "0";
				if(order10<100)
					_factor += "0";

				_factor += _ntoa(order10);
			}
			break;
		case ek_md:
			if(num && _factors10.size())
			{
				int order10 = int(log10(fabs(num))  + (num>1?0.5:-0.5));
				if(order10)
				{
					TMFactors::iterator iter = _factors10.lower_bound(order10);

					if(_factors10.end() == iter)
					{
						iter--;
					}
					else if(iter->first != order10)
					{
						if(_factors10.begin() != iter)
						{
							iter--;
						}
					}

					if(iter->first)
					{
						order10 = iter->first;
						double mult = pow(10.0, order10);
						num /= mult;
						_factor = iter->second;
					}
				}
			}
			break;
		case ek_mb:
			if(num && _factors10.size())
			{
				int order2 = int(log(fabs(num))/log(2.0)  + (num>1?0.5:-0.5));
				if(order2)
				{
					TMFactors::iterator iter = _factors2.lower_bound(order2);

					if(_factors2.end() == iter)
					{
						iter--;
					}
					else if(iter->first != order2)
					{
						if(_factors2.begin() != iter)
						{
							iter--;
						}
					}

					if(iter->first)
					{
						order2 = iter->first;
						double mult = pow(2.0, order2);
						num /= mult;
						_factor = iter->second;
					}
				}
			}
			break;
		default:
			//_expR += 0;
			_factor = "";
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool NumberFormatter::formatNumber_intDigits()
	{
		if(ek_i == _kind)
		{
			_width = std::max(_width, _precision);
			_precision = 0;
		}

		_orderInt = 0;
		if(_expR <= 0)
		{
			//число <1
			_res = "";
		}
		else
		{
			//сформировать значимую часть
			_res.resize(std::max(_expR*4/3, int(_width)));
			int outPos = _res.size()-1;
			int inPos = _expR-1;
			while(inPos>=0)
			{
				if(_delim3 && _orderInt && !(_orderInt%3))
				{
					_res[outPos--] = _delim3;
				}

				_res[outPos--] = inPos<(int)_digits.size() ? _digits[inPos] : '0';
				inPos--;
				_orderInt++;
			}
			outPos++;
			_res.erase(_res.begin(), _res.begin() + outPos);
		}


		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool NumberFormatter::formatNumber_fracDigits()
	{
		if(!_precision)
		{
			if(_res.empty())
			{
				_orderInt++;
				_res = "0";
			}
			return true;
		}

		int zerosAmount;
		int digitsStart;
		int digitsAmount;
		int zerosAmount2;

		if(_expR < 0)
		{
			zerosAmount = -_expR;
			digitsStart = 0;
			digitsAmount = _digits.size();
		}
		else
		{
			zerosAmount = 0;
			digitsStart = _expR;

			if(_expR<(int)_digits.size())
			{
				digitsAmount = _digits.size()-_expR;
			}
			else
			{
				digitsAmount = 0;
			}
		}

		if(_precision > 0)
		{
			if(zerosAmount + digitsAmount < _precision)
			{
				if(_flags & ef2_noPadRightZero)
				{
					zerosAmount2 = 0;
				}
				else
				{
					zerosAmount2 = _precision - (zerosAmount + digitsAmount);
				}
			}
			else if(zerosAmount < _precision)
			{
				zerosAmount2 = 0;
				digitsAmount = _precision - zerosAmount;
			}
			else
			{
				zerosAmount2 = 0;
				digitsAmount = 0;
				zerosAmount = _precision;
			}
		}
		else
		{
			zerosAmount2 = 0;
		}

		//вдруг нет значимых цифр
		bool izZero = true;
		for(int k=digitsStart; k<digitsStart+digitsAmount; k++)
		{
			if(_digits[k] != '0')
			{
				izZero = false;
				break;
			}

		}

		if(izZero)
		{
			if(_res.empty())
			{
				_orderInt++;
				_res = "0";
			}
			return true;
		}


		int totalAmount = zerosAmount + digitsAmount + zerosAmount2;

		if(totalAmount)
		{
			if(_res.empty() && _zero0)
			{
				_orderInt++;
				_res = "0";
			}

			totalAmount++;
			int pos = _res.size();
			_res.resize(_res.size() + totalAmount);

			////
			_res[pos++] = _delim0;

			////
			if(zerosAmount)
			{
				std::fill(_res.begin()+pos, _res.begin()+pos+zerosAmount, '0');
				pos += zerosAmount;
			}

			////
			if(digitsAmount)
			{
				std::copy(_digits.begin()+digitsStart, _digits.begin()+digitsStart+digitsAmount, _res.begin()+pos);
				pos += digitsAmount;
			}

			////
			if(zerosAmount2)
			{
				std::fill(_res.begin()+pos, _res.begin()+pos+zerosAmount2, '0');
				pos += zerosAmount2;
			}

			////срезать правые нули
			if(_flags & ef2_noPadRightZero)
			{
				while(_res.size() && _res[_res.size()-1] == '0')
				{
					_res.erase(_res.end()-1);
				}
			}

		}
		else
		{
			if(_res.empty())
			{
				_orderInt++;
				_res = "0";
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool NumberFormatter::formatNumber_pads()
	{
		//знак, префикс, заполнение до ширины
		char signChar = 0;
		if(_sign<0)
		{
			signChar = '-';
		}
		else
		{
			if(_flags & ef_forcePositivePlus)
			{
				signChar = '+';
			}
			else if(_flags & ef_forcePositiveSpace)
			{
				signChar = ' ';
			}
		}

		if((int)_res.size() < _width - (signChar?1:0) - (int)_factor.size())
		{
			//сколько долить
			int padWidth = (_width - (signChar?1:0) - (int)_factor.size()) - (int)_res.size();

			if(_flags & ef_alignLeft)
			{
				//заполн€ть справа
				if(signChar)
				{
					_res = signChar + _res;
					signChar = 0;
				}

				char padChar;
// 				if(_flags & ef2_noPadRightZero)
				{
					if(_factor.size())
					{
						_res += _factor;
						_factor.clear();
					}
					padChar = ' ';

				}
// 				else
// 				{
// 					padChar = '0';
// 				}
				int pos = _res.size();
				_res.resize(_res.size() + padWidth);
				std::fill(_res.begin()+pos, _res.end(), padChar);
			}
			else
			{
				//заполн€ть слева

				bool padLeftZero = (_flags & ef_padLeftZero)?true:false;
				//если влево дополн€ть не пробелами - то знак поставить сразу
				if(!padLeftZero && signChar)
				{
					_res = signChar + _res;
					signChar = 0;
				}

				while(padWidth>0)
				{
					if(padLeftZero && _delim3 && _orderInt && !(_orderInt%3))
					{
						_res = _delim3+_res;
						padWidth--;
					}

					_res = (padLeftZero?'0':' ') + _res;
					padWidth--;
					_orderInt++;
				}
			}
		}

		if(signChar)
		{
			_res = signChar + _res;
			signChar = 0;
		}

		if(_factor.size())
		{
			_res += _factor;
			_factor.clear();
		}
		return false;
	}

}
