#ifndef _ccms_i18n_h_
#define _ccms_i18n_h_

#include "scripter/jsObject.hpp"
#include "i18n/dicTable.hpp"
#include "i18n/dicTableBucket.hpp"

namespace ccms
{
	class I18n
		: public JsObject
	{
	private:
		bool		_collectUsage;
		jsval		_confs;

		typedef std::map<std::string, DicTablePtr> TMDicTables;
		TMDicTables _dicTables;

		typedef std::set<std::string> TSStrings;
		typedef std::map<std::string, TSStrings > TMTranslationFiles;//lang-> filesSet
		TMTranslationFiles _translationFiles;

	public:
		I18n(bool collectUsage);
		~I18n();

		boost::local_time::tz_database getTzDatabase();
		DicTableBucketPtr mkDicTableBucket(const std::set<std::string> &files, const std::string &language);

	private:
		//reload()
		bool call_reload(uintN argc, jsval *argv, jsval *rval);

		bool xetter_confs(jsval *vp, bool isGet);

		//L10n createL10n(nameOrConf);
		bool call_createL10n(uintN argc, jsval *argv, jsval *rval);

		// addTranslation('ru', 'myFile.po');
		bool call_addTranslation(uintN argc, jsval *argv, jsval *rval);
		// delTranslation('ru', 'myFile.po');
		bool call_delTranslation(uintN argc, jsval *argv, jsval *rval);

		//dumpUsage()
		bool call_dumpUsage(uintN argc, jsval *argv, jsval *rval);

	private:
		void reload(const std::vector<std::string> &dirs);

	private:
		boost::local_time::tz_database _tzDatabase;

	public:
		bool call_serialize(uintN argc, jsval *argv, jsval *rval);
		bool call_deserialize(uintN argc, jsval *argv, jsval *rval);
	};

	typedef JsPtr<I18n> I18nPtr;
}
#endif
