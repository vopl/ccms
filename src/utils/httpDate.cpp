#include "stdafx.h"
#include "utils/httpDate.hpp"
#include "utils/ncvt.h"
#include "utils/mkgmtime.h"

namespace ccms
{
	namespace httpDate
	{
		namespace impl
		{
			static const char *wkday_itoa[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
			static const char *month_itoa[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

			using namespace boost::xpressive;
			//s1 wday
			//s2 month
			//s3 hour
			//s4 minute
			//s5 second
			//s6 day
			//s7 year

	// 		cregex re_wkday = (s1 = (as_xpr("Sun")|"Mon"|"Tue"|"Wed"|"Thu"|"Fri"|"Sat"));
	// 		cregex re_weekday = (s1 = (as_xpr("Sunday")|"Monday"|"Tuesday"|"Wednesday"|"Thursday"|"Friday"|"Saturday"));
	// 		cregex re_month = (s2 = (as_xpr("Jan")|"Feb"|"Mar"|"Apr"|"May"|"Jun"|"Jul"|"Aug"|"Sep"|"Oct"|"Nov"|"Dec"));
	// 
	// 		cregex re_time = (s3=repeat<1,2>(_d)) >> ":" >> (s4=repeat<1,2>(_d)) >> ":" >> (s5=repeat<1,2>(_d));
	// 		cregex re_date3 = re_month >> +as_xpr(" ") >> (s6=repeat<1,2>(_d));
	// 		cregex re_date2 = (s6=repeat<1,2>(_d)) >> "-" >> re_month >> "-" >> (s7=repeat<1,2>(_d));
	// 		cregex re_date1 = (s6=repeat<1,2>(_d)) >> " " >> re_month >> " " >> (s7=repeat<4,4>(_d));

			cregex re_rfc1123 = //re_wkday >> ", " >> (s2=re_date1) >> " " >> re_time >> " GMT";
				(s1 = (as_xpr("Sun")|"Mon"|"Tue"|"Wed"|"Thu"|"Fri"|"Sat")) >> 
				", " >> 
				(s6=repeat<1,2>(_d)) >> " " >> 
					(s2 = (as_xpr("Jan")|"Feb"|"Mar"|"Apr"|"May"|"Jun"|"Jul"|"Aug"|"Sep"|"Oct"|"Nov"|"Dec")) >> 
					" " >> (s7=repeat<4,4>(_d)) >> 
				" " >> 
				(s3=repeat<1,2>(_d)) >> ":" >> (s4=repeat<1,2>(_d)) >> ":" >> (s5=repeat<1,2>(_d)) >> 
				" GMT";

			cregex re_rfc850  = //re_weekday >> ", " >> re_date2 >>" " >> re_time >> " GMT";
				(s1 = (as_xpr("Sun")|"Mon"|"Tue"|"Wed"|"Thu"|"Fri"|"Sat")) >> *_w >>
				", " >> 
				(s6=repeat<1,2>(_d)) >> "-" >> 
					(s2 = (as_xpr("Jan")|"Feb"|"Mar"|"Apr"|"May"|"Jun"|"Jul"|"Aug"|"Sep"|"Oct"|"Nov"|"Dec")) >>
					"-" >> (s7=repeat<1,2>(_d)) >>
				" " >> 
				(s3=repeat<1,2>(_d)) >> ":" >> (s4=repeat<1,2>(_d)) >> ":" >> (s5=repeat<1,2>(_d)) >> 
				" GMT";

			cregex re_asctime = //re_wkday >> " " >> re_date3 >> " " >> re_time >> " " >> (_d>>_d>>_d>>_d);
				(s1 = (as_xpr("Sun")|"Mon"|"Tue"|"Wed"|"Thu"|"Fri"|"Sat")) >> 
				" " >> 
				(s2 = (as_xpr("Jan")|"Feb"|"Mar"|"Apr"|"May"|"Jun"|"Jul"|"Aug"|"Sep"|"Oct"|"Nov"|"Dec")) >> 
					+as_xpr(" ") >> (s6=repeat<1,2>(_d)) >> 
				" " >> 
				(s3=repeat<1,2>(_d)) >> ":" >> (s4=repeat<1,2>(_d)) >> ":" >> (s5=repeat<1,2>(_d)) >> 
				" " >> 
				(s7=(_d>>_d>>_d>>_d));

			static std::map<std::string, int> mwkday = boost::assign::map_list_of
				("Sun",0)
				("Mon",1)
				("Tue",2)
				("Wed",3)
				("Thu",4)
				("Fri",5)
				("Sat",6);

			static std::map<std::string, int> mmonth = boost::assign::map_list_of
				("Jan",0)
				("Feb",1)
				("Mar",2)
				("Apr",3)
				("May",4)
				("Jun",5)
				("Jul",6)
				("Aug",7)
				("Sep",8)
				("Oct",9)
				("Nov",10)
				("Dec",11);
		}


		//////////////////////////////////////////////////////////////////////////
		std::string make(time_t t)
		{
			tm stm;
			gmtime_s(&stm, &t);

			char buf[128];

			sprintf_s(buf, 
				"%s, "
				"%02d %s %04d "
				"%02d:%02d:%02d GMT", 
				impl::wkday_itoa[stm.tm_wday], 
				stm.tm_mday, impl::month_itoa[stm.tm_mon], stm.tm_year+1900,
				stm.tm_hour, stm.tm_min, stm.tm_sec);
			return buf;
		}

		//////////////////////////////////////////////////////////////////////////
		time_t parse(const char *str)
		{
			boost::xpressive::cmatch what;
			struct tm stm = {};
			if(boost::xpressive::regex_match(str, what, impl::re_rfc1123))
			{
				stm.tm_sec = _atoi(what[5].str().data());
				stm.tm_min = _atoi(what[4].str().data());
				stm.tm_hour = _atoi(what[3].str().data());
				stm.tm_mday = _atoi(what[6].str().data());
				stm.tm_mon = impl::mmonth[what[2]];
				stm.tm_year = _atoi(what[7].str().data()) - 1900;
				stm.tm_wday = impl::mwkday[what[1]];
				stm.tm_yday;
				stm.tm_isdst;
			}
			else if(boost::xpressive::regex_match(str, what, impl::re_rfc850))
			{
				stm.tm_sec = _atoi(what[5].str().data());
				stm.tm_min = _atoi(what[4].str().data());
				stm.tm_hour = _atoi(what[3].str().data());
				stm.tm_mday = _atoi(what[6].str().data());
				stm.tm_mon = impl::mmonth[what[2]];
				stm.tm_year = _atoi(what[7].str().data());
				stm.tm_wday = impl::mwkday[what[1]];
				stm.tm_yday;
				stm.tm_isdst;
			}
			else if(boost::xpressive::regex_match(str, what, impl::re_asctime))
			{
				stm.tm_sec = _atoi(what[5].str().data());
				stm.tm_min = _atoi(what[4].str().data());
				stm.tm_hour = _atoi(what[3].str().data());
				stm.tm_mday = _atoi(what[6].str().data());
				stm.tm_mon = impl::mmonth[what[2]];
				stm.tm_year = _atoi(what[7].str().data()) - 1900;
				stm.tm_wday = impl::mwkday[what[1]];
				stm.tm_yday;
				stm.tm_isdst;
			}
			else
			{
				return -1;
			}

			time_t res = mkgmtime(&stm);
			return res;
		}

	}
}