#ifndef _ccms_l10n_h_
#define _ccms_l10n_h_

#include "scripter/jsObject.hpp"
#include "i18n/numberFormatter.hpp"
#include "i18n/timeFormatter.hpp"
#include "i18n/dicTableBucket.hpp"

namespace ccms
{
	class I18n;
	typedef JsPtr<I18n> I18nPtr;

	class L10n
		: public JsObject
	{
		I18nPtr		_i18n;
		jsval		_conf;
		JSObject	*_confNumber;
		JSObject	*_confNumberFormats;
		JSObject	*_confText;
		JSObject	*_confTime;
		JSObject	*_confTimeFormats;

		NumberFormatter		_numberFormatter;
		TimeFormatter		_timeFormatter;
		DicTableBucketPtr	_dtb;
		//////////////////////////////////////////////////////////////////////////

	public:
		L10n(I18nPtr i18n, jsval conf);
		~L10n();

	private:
		//var str = text(id, [plural,] paramsHash);
		bool call_text(uintN argc, jsval *argv, jsval *rval);
		//var str = number(num, format);
		bool call_number(uintN argc, jsval *argv, jsval *rval);
		//var str = time(num, format);
		bool call_time(uintN argc, jsval *argv, jsval *rval);

	private:
		bool xetter_conf(jsval *vp, bool isGet);

	private:
		void setupNumberFormatter(JSObject *conf);
		void setupTimeFormatter(JSObject *conf, const boost::local_time::tz_database &tzDatabase);
		void setupTextFormatter(JSObject *conf);


	private:
		void filesFromDir(std::set<std::string> &files, jsval conf);
		void filesFrom(std::set<std::string> &files, jsval conf, bool stubForDir=false);
	};

	typedef JsPtr<L10n> L10nPtr;
}
#endif
