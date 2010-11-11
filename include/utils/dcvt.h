#ifndef _ccms_dcvt_h_
#define _ccms_dcvt_h_

namespace ccms
{
	enum EDcvtRes
	{
		edr_ok,
		edr_nan,
		edr_inf,
		edr_den,
		edr_badRadix,
		edr_lowBuffer,
	};


	EDcvtRes dcvt(
		double d, 
		size_t radix,
		unsigned char *digits, 
		int &digitsAmount,
		int &expR,
		int &sign);

}

#endif
