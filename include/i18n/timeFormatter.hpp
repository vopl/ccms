#ifndef _ccms_timeFormatter_h_
#define _ccms_timeFormatter_h_

#include "boost/date_time/local_time/local_time.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace ccms
{
	//потоконебезопасный - 18 букв
	class TimeFormatter
	{


	public:
		typedef std::vector<std::string> TVString;

	public:
		TimeFormatter();
		void setup(
			boost::local_time::time_zone_ptr tz,
			const TVString &dowShort,
			const TVString &dowLong,
			const TVString &monthShort,
			const TVString &monthLong,
			const std::string &am,
			const std::string &pm,
			const std::string &AM,
			const std::string &PM);
		std::string format(boost::posix_time::ptime utc, const char *format);

	private:
		boost::local_time::time_zone_ptr _tz;
		TVString	_dowShort;
		TVString	_dowLong;
		TVString	_monthLong;
		TVString	_monthShort;
		std::string	_am;
		std::string	_pm;
		std::string	_AM;
		std::string	_PM;
	};
}

#endif
