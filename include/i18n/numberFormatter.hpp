#ifndef _ccms_numberFormatter_h_
#define _ccms_numberFormatter_h_

namespace ccms
{
	//������������������ - 18 ����
	class NumberFormatter
	{
	public:
		NumberFormatter();

		typedef std::map<int, std::string> TMFactors;
		void setup(
			char delim0, 
			char delim3, 
			bool zero0, 
			const char *nan, 
			const char *inf,
			const TMFactors &factors10,
			const TMFactors &factors2);
		std::string format(double num, const char *format);

	private:
		//��������� ������
		char _delim0;
		char _delim3;
		bool _zero0;
		std::string _nan;
		std::string _inf;
		TMFactors _factors10;
		TMFactors _factors2;

	private:
		boost::xpressive::cregex _re;
		boost::xpressive::cmatch _what;

		//����� � �����2
		enum EFlags
		{
			ef_null = 0x0,
			ef_alignLeft=0x01,				//��������� ����� ��������
			ef_padLeftZero=0x02,			//��������� ����� �����
			ef_forcePositivePlus=0x04,		//��� ������������� �������� +
			ef_forcePositiveSpace=0x08,		//��� ������������� �������� ������
			ef_padRight=0x10,				//��������� ������ ��������
			ef_mask = 0xff,

			ef2_noPadRightZero=0x0100,			//�� ��������� ������ �����
			ef2_mask = 0xff00,
		};
		enum EKind
		{
			ek_i,
			ek_f,
			ek_e,
			ek_E,
			ek_g,
			ek_G,
			ek_md,
			ek_mb,
		};


		int		_flags;
		int		_width;
		int		_precision;
		EKind	_kind;
		size_t	_radix;
		bool	_radixUppercase;


	private:
		std::string _res;
		std::vector<unsigned char> _digits;
		int _expR;
		int _sign; 

		//��������� ������������� ��������� ��� s, e
		std::string _factor;

		//���������� ���������� ����� ��������, ������������ ��� ������������ ����������� 3 ��������
		int _orderInt;

		static const char _abc[37];
		static const char _ABC[37];


	private:
		bool recognizeFormat(const char *format);
	private:
		bool formatNumber_prepareFactor(double &num);
		bool formatNumber_intDigits();
		bool formatNumber_fracDigits();
		bool formatNumber_pads();
	};
}

#endif

