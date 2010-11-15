#ifndef _ccms_httpDate_h_
#define _ccms_httpDate_h_

namespace ccms
{
	namespace httpDate
	{
		//////////////////////////////////////////////////////////////////////////
		//RFC 822, updated by RFC 1123
		std::string make(time_t t);

		//////////////////////////////////////////////////////////////////////////
		/*
		   HTTP-date    = rfc1123-date | rfc850-date | asctime-date
		   rfc1123-date = wkday "," SP date1 SP time SP "GMT"
		   rfc850-date  = weekday "," SP date2 SP time SP "GMT"
		   asctime-date = wkday SP date3 SP time SP 4DIGIT
		   date1        = 2DIGIT SP month SP 4DIGIT
						  ; day month year (e.g., 02 Jun 1982)
		   date2        = 2DIGIT "-" month "-" 2DIGIT
						  ; day-month-year (e.g., 02-Jun-82)
		   date3        = month SP ( 2DIGIT | ( SP 1DIGIT ))
						  ; month day (e.g., Jun  2)
		   time         = 2DIGIT ":" 2DIGIT ":" 2DIGIT
						  ; 00:00:00 - 23:59:59
		   wkday        = "Mon" | "Tue" | "Wed"
						| "Thu" | "Fri" | "Sat" | "Sun"
		   weekday      = "Monday" | "Tuesday" | "Wednesday"
						| "Thursday" | "Friday" | "Saturday" | "Sunday"
		   month        = "Jan" | "Feb" | "Mar" | "Apr"
						| "May" | "Jun" | "Jul" | "Aug"
						| "Sep" | "Oct" | "Nov" | "Dec"
		*/
		time_t parse(const char *str);
	}

}

#endif
