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


			//День месяца, 2 цифры с ведущими нулями
 			case 'd': append(res, ld.day(), 2); break;
			//D Сокращенное наименование дня недели, 3 символа от Mon до Sun 
 			case 'D': res += _dowShort[ld.day_of_week()]; break;
			//j День месяца без ведущих нулей От 1 до 31 
			case 'j': append(res, ld.day(), 0); break;
			//l (строчная 'L') Полное наименование дня недели От Sunday до Saturday 
			case 'l': res += _dowLong[ld.day_of_week()]; break;
			//w Порядковый номер дня недели От 0 (воскресенье) до 6 (суббота) 
			case 'w': append(res, ld.day_of_week(), 0); break;
			//N Порядковый номер дня недели От 1 (понедельник) до 7 (воскресенье) 
			case 'N': {int dow = ld.day_of_week(); append(res, dow?dow:7, 0);} break;
			//z Порядковый номер дня в году (нумерация с 0) От 0 до 365 
			case 'z': append(res, ld.day_of_year()-1, 0); break;
 



			//W Порядковый номер недели года по ISO-8601, первый день недели - понедельник. Например: 42 (42-я неделя года) 
			case 'W': append(res, ld.week_number(), 0); break;
			//o ISO год
			//case 'o': append(res, ld.year(), 0); break;



			//F Полное наименование месяца, например January или March от January до December 
			case 'F': res += _monthLong[ld.month()-1]; break;
			//m Порядковый номер месяца с ведущими нулями От 01 до 12 
			case 'm': append(res, ld.month(), 2); break;
			//M Сокращенное наименование месяца, 3 символа От Jan до Dec 
			case 'M': res += _monthShort[ld.month()-1]; break;
			//n Порядковый номер месяца без ведущих нулей От 1 до 12 
			case 'n': append(res, ld.month(), 0); break;
			//t Количество дней в месяце От 28 до 31 
			case 't': append(res, ld.end_of_month().day(), 0); break;


			//L Признак високосного года 1, если год високосный, иначе 0. 
			case 'L': res += gregorian_calendar::is_leap_year(ld.year())?'1':'0'; break;
			//y Номер года, 2 цифры Примеры: 99, 03 
			case 'y': append(res, ld.year()%100, 2); break;
			//Y Порядковый номер года, 4 цифры Примеры: 1999, 2003 
			case 'Y': append(res, ld.year(), 4); break;



			//a Ante meridiem или Post meridiem в нижнем регистре am или pm 
 			case 'a': res += lt.hours() >=12 ? _pm : _am; break;
			//A Ante meridiem или Post meridiem в верхнем регистре AM или PM 
			case 'A': res += lt.hours() >=12 ? _PM : _AM; break;
			//B Время в стадарте Swatch Internet От 000 до 999 
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
			//g Часы в 12-часовом формате без ведущих нулей От 1 до 12 
			case 'g': {int h = lt.hours(); append(res, (h % 12) ? h % 12 : 12, 0);} break;
			//G Часы в 24-часовом формате без ведущих нулей От 0 до 23 
			case 'G': append(res, lt.hours(), 0); break;
			//h Часы в 12-часовом формате с ведущими нулями От 01 до 12 
			case 'h': {int h = lt.hours(); append(res, (h % 12) ? h % 12 : 12, 2);} break;
			//H Часы в 24-часовом формате с ведущими нулями От 00 до 23 
			case 'H': append(res, lt.hours(), 2); break;
			//i Минуты с ведущими нулями 00 to 59 
			case 'i': append(res, lt.minutes(), 2); break;
			//s Секунды с ведущими нулями От 00 до 59 
			case 's': append(res, lt.seconds(), 2); break;




			//I (заглавная i) Признак летнего времени 1, если дата соответствует летнему времени, иначе 0 otherwise. 
			case 'I': res += ldt.is_dst()?'1':'0'; break;
			//P Разница с временем по Гринвичу в часах и минутах Например: +02:15 
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
			//O Разница с временем по Гринвичу в часах Например: +0200 
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

			//T Временная зона аббревиатура. Примеры: EST, MDT ... 
			case 'T': res += ldt.zone_abbrev(); break;
			//e Временная зона имя.
			case 'e': res += ldt.zone_name(); break;
			//Z Смещение временной зоны в секундах. Для временных зон западнее UTC это отрицательное число, восточнее UTC - положительное. От -43200 до 43200 
			case 'Z': 
				{
					int seconds = ldt.zone()->base_utc_offset().total_seconds();
					if(ldt.is_dst())seconds += ldt.zone()->dst_offset().total_seconds();
					append(res, seconds);
				}
				break;



			//c Дата в формате ISO 8601 (добавлено в PHP 5) 2004-02-12T15:19:21+00:00 
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
			//r Дата в формате RFC 2822 Например: Thu, 21 Dec 2000 16:01:07 +0200 
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
			//U Количество секунд, прошедших с начала Эпохи Unix (The Unix Epoch, 1 января 1970, 00:00:00 GMT)
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

