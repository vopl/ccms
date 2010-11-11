#include "stdafx.h"
#include "utils/dcvt.h"
#include <boost/math/special_functions/fpclassify.hpp>

namespace ccms
{
	EDcvtRes dcvt(
		double d, 
		size_t radix,
		unsigned char *digits, 
		int &digitsAmount,
		int &expR,
		int &sign)
	{
		switch(boost::math::fpclassify(d))
		{
		case FP_SUBNORMAL:
			return edr_den;
		case FP_INFINITE:
			return edr_inf;
		case FP_NAN:
			return edr_nan;

		default:
		case FP_NORMAL:
		case FP_ZERO:
			//ok
			break;
		}

		if(radix<2)
		{
			return edr_badRadix;
		}

		unsigned char *p = digits;

		if(!d)
		{
			digitsAmount = 0;
			expR = 0;
			return edr_ok;
		}


		//вы€вить мантиссу и двоичную экспоненту

		typedef unsigned long long ull;
		//static assert sizeof(ull) = sizeof(double)
		union
		{
			ull ll;
			double d; 
		} lld;
		lld.d = d;

		sign = (lld.ll & 0x8000000000000000ull)? -1: 1;
		ull mantissa = (lld.ll & 0xFFFFFFFFFFFFFull) | 0x10000000000000ull; 
		int exp2 = (signed short)((lld.ll >> 52) & 0x7FF) - 0x3ff; 

		//////////////////////////////////////////////////////////////////////////
		//скорректироват на ближайшую целую степень основани€
		expR = int(log(pow(2.0, exp2)) / log(double(radix)));

		double mmult = pow(2.0, exp2) / pow(double(radix), expR);
		ull rmantissa = rmantissa = ull(mmult * double(mantissa));
		while(1)
		{
			unsigned char digit = (unsigned char)(rmantissa >> 52);
			if(!digit)
			{
				expR--;
				mmult = pow(2.0, exp2) / pow(double(radix), expR);
				rmantissa = ull(mmult * double(mantissa));
			}
			else if(digit >= radix)
			{
				expR++;
				mmult = pow(2.0, exp2) / pow(double(radix), expR);
				rmantissa = ull(mmult * double(mantissa));
				break;
			}
			else
			{
				break;
			}
		}
		expR++;

		//количество значащих цифр
		size_t sdigits = size_t(log(4503599627370496.0)/log(double(radix)));

		for(size_t cnt(0); cnt<=sdigits; cnt++)
		{
			unsigned char digit = (unsigned char)(rmantissa >> 52);
			if(p-digits >= digitsAmount) return edr_lowBuffer;
			*p++ = digit;

			rmantissa &= 0xFFFFFFFFFFFFFull; 
			rmantissa *= radix;
		}

		//////////////////////////////////////////////////////////////////////////
		if(radix>=4 && (p-digits > 4))
		{
			//если предпоследние две крайние цифры и последн€€ - склон€етс€ к краю то округлить
			unsigned char d0 = 0;
			unsigned char dn2 = radix/2;
			unsigned char dn = radix-1;

			if(	*(p-1) < dn2 &&
				*(p-2) == d0 &&
				*(p-3) == d0)
			{
				p--;
				p--;
				p--;
				while(*p == d0 && p>digits)
				{
					p--;
				}
				p++;
			}
			if(	*(p-1) >= dn2 &&
				*(p-2) == dn &&
				*(p-3) == dn)
			{
				p--;
				p--;
				p--;
				while(*p == dn && p>digits)
				{
					p--;
				}
				if(p-digits >= digitsAmount) return edr_lowBuffer;
				*p += 1;
				if(*p == radix)
				{
					*p = 1;
					expR++;
				}
				p++;
			}
		}
		else
		{
			if((p-digits > 4))
			{
				p--;//минус один разр€д
			}
		}

		//пока последний ноль - резать
		if((p-digits > 4))
		{
			unsigned char d0 = 0;
			p--;
			while(*p == d0 && p>digits)
			{
				p--;
			}
			p++;
		}

		//количество цифр на выходе
		digitsAmount = p-digits;

		return edr_ok;
	}
}
