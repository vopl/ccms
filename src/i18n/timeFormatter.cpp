#include "stdafx.h"
#include "i18n/timeFormatter.hpp"
#include "utils/ncvt.h"

namespace ccms
{
	using namespace boost::gregorian; 
	using namespace boost::local_time;
	using namespace boost::posix_time;

	//////////////////////////////////////////////////////////////////////////
	TimeFormatter::TimeFormatter()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void TimeFormatter::setup(
		time_zone_ptr tz,
		const TVString &dowShort,
		const TVString &dowLong,
		const TVString &monthShort,
		const TVString &monthLong,
		const std::string &am,
		const std::string &pm,
		const std::string &AM,
		const std::string &PM)

	{
		_tz = tz;
		_dowShort = dowShort; _dowShort.resize(7);
		_dowLong = dowLong; _dowLong.resize(7);
		_monthLong = monthLong; _monthLong.resize(12);
		_monthShort = monthShort; _monthShort.resize(12);
		_am = am;
		_pm = pm;
		_AM = AM;
		_PM = PM;
	}


	void append(std::string &res, int v, int digits=0)
	{
		int zeros = 0;

		switch(digits)
		{
		case 0:
			zeros = 0;
			break;
		case 2:
			if(v<10		) zeros = 1;
			else zeros = 0;
			break;
		case 3:
			if(v<10		) zeros = 2;
			else if(v<100		) zeros = 1;
			else zeros = 0;
			break;
		case 4:
			if(v<10		) zeros = 3;
			else if(v<100		) zeros = 2;
			else if(v<1000		) zeros = 1;
			else zeros = 0;
			break;
		case 5:
			if(v<10		) zeros = 4;
			else if(v<100		) zeros = 3;
			else if(v<1000		) zeros = 2;
			else if(v<10000	) zeros = 1;
			else zeros = 0;
			break;
		case 6:
			if(v<10		) zeros = 5;
			else if(v<100		) zeros = 4;
			else if(v<1000		) zeros = 3;
			else if(v<10000	) zeros = 2;
			else if(v<100000	) zeros = 1;
			else zeros = 0;
			break;
		default:
			assert(0);
		}

		for(int i(0); i<zeros; i++) res += '0';
		res += _ntoa(v);
	}

	//////////////////////////////////////////////////////////////////////////
	std::string TimeFormatter::format(ptime utc, const char *format)
	{
		ptime _epoch(date(1970,1,1)); 
		if (!format || !format[0])
		{
			return "";
		}

		local_date_time ldt(utc, _tz);
		ptime pldt = ldt.local_time();
		date ld = pldt.date();
		time_duration lt = pldt.time_of_day();

		std::string res;


 		int formatSize = strlen(format);
 		for (int i = 0; i < formatSize; i++)
 		{
 			switch (format[i])
 			{


			//���� ������, 2 ����� � �������� ������
 			case 'd': append(res, ld.day(), 2); break;
			//D ����������� ������������ ��� ������, 3 ������� �� Mon �� Sun 
 			case 'D': res += _dowShort[ld.day_of_week()]; break;
			//j ���� ������ ��� ������� ����� �� 1 �� 31 
			case 'j': append(res, ld.day(), 0); break;
			//l (�������� 'L') ������ ������������ ��� ������ �� Sunday �� Saturday 
			case 'l': res += _dowLong[ld.day_of_week()]; break;
			//w ���������� ����� ��� ������ �� 0 (�����������) �� 6 (�������) 
			case 'w': append(res, ld.day_of_week(), 0); break;
			//N ���������� ����� ��� ������ �� 1 (�����������) �� 7 (�����������) 
			case 'N': {int dow = ld.day_of_week(); append(res, dow?dow:7, 0);} break;
			//z ���������� ����� ��� � ���� (��������� � 0) �� 0 �� 365 
			case 'z': append(res, ld.day_of_year()-1, 0); break;
 



			//W ���������� ����� ������ ���� �� ISO-8601, ������ ���� ������ - �����������. ��������: 42 (42-� ������ ����) 
			case 'W': append(res, ld.week_number(), 0); break;
			//o ISO ���
			//case 'o': append(res, ld.year(), 0); break;



			//F ������ ������������ ������, �������� January ��� March �� January �� December 
			case 'F': res += _monthLong[ld.month()-1]; break;
			//m ���������� ����� ������ � �������� ������ �� 01 �� 12 
			case 'm': append(res, ld.month(), 2); break;
			//M ����������� ������������ ������, 3 ������� �� Jan �� Dec 
			case 'M': res += _monthShort[ld.month()-1]; break;
			//n ���������� ����� ������ ��� ������� ����� �� 1 �� 12 
			case 'n': append(res, ld.month(), 0); break;
			//t ���������� ���� � ������ �� 28 �� 31 
			case 't': append(res, ld.end_of_month().day(), 0); break;


			//L ������� ����������� ���� 1, ���� ��� ����������, ����� 0. 
			case 'L': res += gregorian_calendar::is_leap_year(ld.year())?'1':'0'; break;
			//y ����� ����, 2 ����� �������: 99, 03 
			case 'y': append(res, ld.year()%100, 2); break;
			//Y ���������� ����� ����, 4 ����� �������: 1999, 2003 
			case 'Y': append(res, ld.year(), 4); break;



			//a Ante meridiem ��� Post meridiem � ������ �������� am ��� pm 
 			case 'a': res += lt.hours() >=12 ? _pm : _am; break;
			//A Ante meridiem ��� Post meridiem � ������� �������� AM ��� PM 
			case 'A': res += lt.hours() >=12 ? _PM : _AM; break;
			//B ����� � �������� Swatch Internet �� 000 �� 999 
			case 'B':
				{
					long long sse = (utc - _epoch).total_seconds();
					int retval = (int)(((sse-(sse - ((sse % 86400) + 3600))) * 10) / 864);
					while (retval < 0)
					{
						retval += 1000;
					}
					retval = retval % 1000;
					append(res, retval, 3); break;
				}
				break;
			//g ���� � 12-������� ������� ��� ������� ����� �� 1 �� 12 
			case 'g': {int h = lt.hours(); append(res, (h % 12) ? h % 12 : 12, 0);} break;
			//G ���� � 24-������� ������� ��� ������� ����� �� 0 �� 23 
			case 'G': append(res, lt.hours(), 0); break;
			//h ���� � 12-������� ������� � �������� ������ �� 01 �� 12 
			case 'h': {int h = lt.hours(); append(res, (h % 12) ? h % 12 : 12, 2);} break;
			//H ���� � 24-������� ������� � �������� ������ �� 00 �� 23 
			case 'H': append(res, lt.hours(), 2); break;
			//i ������ � �������� ������ 00 to 59 
			case 'i': append(res, lt.minutes(), 2); break;
			//s ������� � �������� ������ �� 00 �� 59 
			case 's': append(res, lt.seconds(), 2); break;




			//I (��������� i) ������� ������� ������� 1, ���� ���� ������������� ������� �������, ����� 0 otherwise. 
			case 'I': res += ldt.is_dst()?'1':'0'; break;
			//P ������� � �������� �� �������� � ����� � ������� ��������: +02:15 
 			case 'P':
				{
					int minutes = ldt.zone()->base_utc_offset().total_seconds()/60;
					if(ldt.is_dst())minutes += ldt.zone()->dst_offset().total_seconds()/60;
					if(minutes<0)
					{
						minutes = -minutes;
						res += '-';
					}
					else
					{
						res += '+';
					}
					append(res, minutes/60, 2);
					res += ':';
					append(res, minutes%60, 2);
				}
				break;
			//O ������� � �������� �� �������� � ����� ��������: +0200 
			case 'O': 
				{
					int minutes = ldt.zone()->base_utc_offset().total_seconds()/60;
					if(ldt.is_dst())minutes += ldt.zone()->dst_offset().total_seconds()/60;
					if(minutes<0)
					{
						minutes = -minutes;
						res += '-';
					}
					else
					{
						res += '+';
					}
					append(res, minutes/60, 2);
					append(res, minutes%60, 2);
				}
				break;

			//T ��������� ���� ������������. �������: EST, MDT ... 
			case 'T': res += ldt.zone_abbrev(); break;
			//e ��������� ���� ���.
			case 'e': res += ldt.zone_name(); break;
			//Z �������� ��������� ���� � ��������. ��� ��������� ��� �������� UTC ��� ������������� �����, ��������� UTC - �������������. �� -43200 �� 43200 
			case 'Z': 
				{
					int seconds = ldt.zone()->base_utc_offset().total_seconds();
					if(ldt.is_dst())seconds += ldt.zone()->dst_offset().total_seconds();
					append(res, seconds);
				}
				break;



			//c ���� � ������� ISO 8601 (��������� � PHP 5) 2004-02-12T15:19:21+00:00 
			case 'c':
				{
					append(res, ld.year(), 4);
					res += '-';
					append(res, ld.month(), 2);
					res += '-';
					append(res, ld.day(), 2);
					res += 'T';
					append(res, lt.hours(), 2);
					res += ':';
					append(res, lt.minutes(), 2);
					res += ':';
					append(res, lt.seconds(), 2);

					int minutes = ldt.zone()->base_utc_offset().total_seconds()/60;
					if(ldt.is_dst())minutes += ldt.zone()->dst_offset().total_seconds()/60;

					if(minutes<0)
					{
						minutes = -minutes;
						res += '-';
					}
					else
					{
						res += '+';
					}
					append(res, minutes/60, 2);
					res += ':';
					append(res, minutes%60, 2);

				}
				break;
			//r ���� � ������� RFC 2822 ��������: Thu, 21 Dec 2000 16:01:07 +0200 
			case 'r':
				{
					static const char *dows[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
					res += dows[ld.day_of_week()];
					res += ", ";

					append(res, ld.day(), 2);
					res += ' ';

					static const char *months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Oct","Nov","Dec"};
					res += months[ld.month()-1];
					res += ' ';

					append(res, ld.year(), 4);
					res += ' ';

					append(res, lt.hours(), 2);
					res += ':';
					append(res, lt.minutes(), 2);
					res += ':';
					append(res, lt.seconds(), 2);
					res += ' ';

					int minutes = ldt.zone()->base_utc_offset().total_seconds()/60;
					if(ldt.is_dst())minutes += ldt.zone()->dst_offset().total_seconds()/60;

					if(minutes<0)
					{
						minutes = -minutes;
						res += '-';
					}
					else
					{
						res += '+';
					}
					append(res, minutes/60, 2);
					append(res, minutes%60, 2);
				}
				break;
			//U ���������� ������, ��������� � ������ ����� Unix (The Unix Epoch, 1 ������ 1970, 00:00:00 GMT)
			case 'U':
				{
					long long sse = (utc - _epoch).total_seconds();
					res += _ntoa(sse);
				}
				break;
 			case '\\':
				if(i < formatSize)
					i++;
				//break;
 			default:
				res += format[i];
				break;
 			}
 		}

		return res;
	}
}

