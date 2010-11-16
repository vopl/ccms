#ifndef _ccms_ncvt_h_
#define _ccms_ncvt_h_

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	namespace impl
	{
		template <int maxDecs, typename integral>
		std::string _ntoa(integral num)
		{
			if(num)
			{
				bool negative = num<0;
				std::string res;
				res.resize(maxDecs, ' ');
				int pos=maxDecs;

				if(negative)
				{
					while(num)
					{
						res[--pos] = (char)('0' - (num % 10));
						num /= 10;
					}
					res[--pos] = '-';
				}
				else
				{
					while(num)
					{
						res[--pos] = (char)('0' + (num % 10));
						num /= 10;
					}
				}
				res.erase(0, pos);
				return res;
			}
			return "0";
		}
		template <int maxDecs, typename integral>
		std::string _untoa(integral num)
		{
			if(num)
			{
				std::string res;
				res.resize(maxDecs, ' ');
				int pos=maxDecs;

				while(num)
				{
					res[--pos] = (char)('0' + (num % 10));
					num /= 10;
				}
				res.erase(0, pos);
				return res;
			}
			return "0";
		}

		//////////////////////////////////////////////////////////////////////////
		template <typename integral>
		void _aton(const char * a, integral &num)
		{
			num = 0;
			bool negative = false;

			if(a[0] == '-')
			{
				negative = true;
				a++;
			}

			while(*a>='0' && *a<='9')
			{
				num *= 10;
				num += *a - '0';
				a++;
			}
			if(negative)
			{
				num = -num;
			}
		}
		template <typename integral>
		void _atoun(const char * a, integral &num)
		{
			num = 0;
			while(*a>='0' && *a<='9')
			{
				num *= 10;
				num += *a - '0';
				a++;
			}
		}
	}

	inline
		std::string _ntoa(long long num)
	{
		return impl::_ntoa<24>(num);
	}
	inline
		std::string _ntoa(unsigned long long num)
	{
		return impl::_untoa<25>(num);
	}
	inline
		std::string _ntoa(long num)
	{
		return impl::_ntoa<12>(num);
	}
	inline
		std::string _ntoa(unsigned long num)
	{
		return impl::_untoa<13>(num);
	}
	inline
		std::string _ntoa(int num)
	{
		return impl::_ntoa<12>(num);
	}
	inline
		std::string _ntoa(unsigned int num)
	{
		return impl::_untoa<13>(num);
	}
	inline
		std::string _ntoa(short num)
	{
		return impl::_ntoa<6>(num);
	}
	inline
		std::string _ntoa(unsigned short num)
	{
		return impl::_untoa<7>(num);
	}
	inline
		std::string _ntoa(char num)
	{
		return impl::_ntoa<3>(num);
	}
	inline
		std::string _ntoa(unsigned char num)
	{
		return impl::_untoa<4>(num);
	}

	//////////////////////////////////////////////////////////////////////////
	inline char _atoc(const char * a)
	{
		char res;
		impl::_aton(a, res);
		return res;
	}
	inline unsigned char _atouc(const char * a)
	{
		unsigned char res;
		impl::_atoun(a, res);
		return res;
	}
	inline short _atos(const char * a)
	{
		short res;
		impl::_aton(a, res);
		return res;
	}
	inline unsigned short _atous(const char * a)
	{
		unsigned short res;
		impl::_atoun(a, res);
		return res;
	}
	inline int _atoi(const char * a)
	{
		int res;
		impl::_aton(a, res);
		return res;
	}
	inline unsigned int _atoui(const char * a)
	{
		unsigned int res;
		impl::_atoun(a, res);
		return res;
	}
	inline long _atol(const char * a)
	{
		long res;
		impl::_aton(a, res);
		return res;
	}
	inline unsigned long _atoul(const char * a)
	{
		unsigned long res;
		impl::_atoun(a, res);
		return res;
	}
	inline long long _atoll(const char * a)
	{
		long long res;
		impl::_aton(a, res);
		return res;
	}
	inline unsigned long long _atoull(const char * a)
	{
		unsigned long long res;
		impl::_atoun(a, res);
		return res;
	}
	inline size_t _atost(const char * a)
	{
		size_t res;
		impl::_atoun(a, res);
		return res;
	}
	inline time_t _atot(const char * a)
	{
		time_t res;
		impl::_atoun(a, res);
		return res;
	}




	inline std::string _htoa(size_t h)
	{
		std::string res;
		if(!h)
		{
			res = "0";
		}
		else
		{
			size_t h1 = h;
			size_t digits = 0;
			while(h1)
			{
				digits++;
				h1 >>= 4;
			}
			res.resize(digits);

			h1 = h;
			while(h1)
			{
				unsigned char v = h1&0xf;
				if(v < 10) v = v+'0';
				else v = v-10+'A';

				res[--digits] = v;
				h1 >>= 4;
			}
		}

		return res;
	}
}

#endif
