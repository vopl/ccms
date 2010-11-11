#include <stdafx.h>
#include "i18n/l10n.hpp"
#include "i18n/i18n.hpp"
#include "scripter/scripter.hpp"

namespace ccms
{







	//////////////////////////////////////////////////////////////////////////
	L10n::L10n(I18nPtr i18n, jsval conf)
		: JsObject(true, "[object L10n]")
		, _i18n(i18n)
		, _conf(conf)
		, _confNumber(NULL)
		, _confNumberFormats(NULL)
		, _confText(NULL)
		, _confTime(NULL)
		, _confTimeFormats(NULL)
	{
		(JSExceptionReporter)JS_AddNamedRoot(ecx()->_jsCx, &_conf, "L10n::_conf");
		jsval confPart;
		(JSExceptionReporter)JS_GetProperty(ecx()->_jsCx, JSVAL_TO_OBJECT(_conf), "text", &confPart);
		_confText = JSVAL_TO_OBJECT(confPart);


		(JSExceptionReporter)JS_GetProperty(ecx()->_jsCx, JSVAL_TO_OBJECT(_conf), "number", &confPart);
		_confNumber = JSVAL_TO_OBJECT(confPart);
		(JSExceptionReporter)JS_GetProperty(ecx()->_jsCx, _confNumber, "formats", &confPart);
		if(JSVAL_IS_OBJECT(confPart)) _confNumberFormats = JSVAL_TO_OBJECT(confPart);


		(JSExceptionReporter)JS_GetProperty(ecx()->_jsCx, JSVAL_TO_OBJECT(_conf), "time", &confPart);
		_confTime = JSVAL_TO_OBJECT(confPart);
		(JSExceptionReporter)JS_GetProperty(ecx()->_jsCx, _confTime, "formats", &confPart);
		if(JSVAL_IS_OBJECT(confPart)) _confTimeFormats = JSVAL_TO_OBJECT(confPart);



		jsRegisterMeth("text", &L10n::call_text, 1);
		jsRegisterMeth("number", &L10n::call_number, 1);
		jsRegisterMeth("time", &L10n::call_time, 1);

		jsRegisterProp("conf", &L10n::xetter_conf, true);

		setupNumberFormatter(_confNumber);
		setupTimeFormatter(_confTime, _i18n->getTzDatabase());
		setupTextFormatter(_confText);


	}

	//////////////////////////////////////////////////////////////////////////
	L10n::~L10n()
	{
		(JSExceptionReporter)JS_RemoveRoot(ecx()->_jsCx, &_conf);
	}

	//////////////////////////////////////////////////////////////////////////
	bool L10n::call_text(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1 || argc > 4)
		{
			JS_ReportError(ecx()->_jsCx, "[L10n.text must be called with 1-4 arguments]");
			return false;
		}

		JSContext *cx = ecx()->_jsCx;
		const char *id0=NULL;
		const char *id1=NULL;
		int32 plural=0;
		JSObject *conf = NULL;

		if(!JS_ConvertArguments(cx, 1, argv+0, "s", &id0))return false;
		if(argc <= 2)
		{
			id0 = _dtb->translate(id0);
		}
		else
		{
			if(!JS_ConvertArguments(cx, 2, argv+1, "si", &id1, &plural))return false;
			if(plural<0)plural=-plural;
			id0 = _dtb->translate(id0, id1, plural);
		}

		if(2==argc)
		{
			if(!JS_ConvertArguments(cx, 1, argv+1, "o", &conf))return false;
		}
		else if(4==argc)
		{
			if(!JS_ConvertArguments(cx, 1, argv+3, "o", &conf))return false;
		}

		if(conf)
		{
			JSIdArray *arr = JS_Enumerate(cx, conf);
			if(arr->length)
			{
				std::string res = id0;
				for(jsint i(0); i<arr->length; i++)
				{
					jsval jsv[2];
					if(!JS_IdToValue(cx, arr->vector[i], jsv+0))return false;
					if(!JS_GetPropertyById(cx, conf, arr->vector[i], jsv+1))return false;

					char *key, *value;
					if(!JS_ConvertArguments(ecx()->_jsCx, 2, jsv, "ss", &key, &value))return false;

					size_t keyLen = strlen(key);
					if(keyLen)
					{
						size_t pos = res.find(key);
						while(pos < res.size())
						{
							res.replace(res.begin()+pos, res.begin()+pos+keyLen, value);
							pos = res.find(key, pos+keyLen);
						}
					}
				}
				*rval = STRING_TO_JSVAL(JS_NewStringCopyN(cx, res.c_str(), res.size()));
				return true;
			}
			JS_DestroyIdArray(ecx()->_jsCx, arr);

		}

		*rval = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, id0));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool L10n::call_number(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[L10n.number must be called with 1 argument]");
			return false;
		}

		jsval num = argv[0];
		jsval format = JSVAL_VOID;
		uintN extraArgc = 0;
		jsval *extraArgv = NULL;
		if(argc > 1)
		{
			format = argv[1];
		}
		if(argc > 2)
		{
			extraArgc = argc-2;
			extraArgv = argv+2;
		}

		//если строка и есть такой атрибут в конфе - то взять его значение
		if(JSVAL_IS_STRING(format))
		{
			if(_confNumberFormats)
			{
				jsid id;
				if(!JS_ValueToId(ecx()->_jsCx, format, &id))return false;
				JSBool found;
				if(JS_HasPropertyById(ecx()->_jsCx, _confNumberFormats, id, &found) && found)
				{
					if(!JS_GetPropertyById(ecx()->_jsCx, _confNumberFormats, id, &format))return false;
				}
			}
		}

		//если строка - то трактовать ее как формат
		if(JSVAL_IS_STRING(format))
		{
			char *strFormat = NULL;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &format, "s", &strFormat))return false;

			double dnum;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &num, "d", &dnum))return false;

			std::string result = _numberFormatter.format(dnum, strFormat);
			*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, result.c_str(), result.size()));
			return true;
		}

		//если функция - то вызвать ее с дополнительными параметрами
		if(JSVAL_IS_OBJECT(format))
		{
			JSObject *formatObject = JSVAL_TO_OBJECT(format);
			if(JS_ObjectIsFunction(ecx()->_jsCx, formatObject))
			{
				if(extraArgc)
				{
					std::vector<jsval> fargv(extraArgc+1);
					fargv[0] = num;
					std::copy(extraArgv, extraArgv+extraArgc, fargv.begin()+1);
					return JS_CallFunctionValue(ecx()->_jsCx, _confNumber, format, fargv.size(), &fargv[0], rval)?true:false;
				}
				else
				{
					return JS_CallFunctionValue(ecx()->_jsCx, _confNumber, format, 1, &num, rval)?true:false;
				}
			}
			else
			{
				//не функция, игнорировать

			}
		}

		//ничего не прошло, привести num к строке
		JSString *str;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &num, "S", &str))return false;
		*rval = STRING_TO_JSVAL(str);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool L10n::call_time(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[L10n.time must be called with 1 argument]");
			return false;
		}

		jsval time = argv[0];
		jsval format = JSVAL_VOID;
		uintN extraArgc = 0;
		jsval *extraArgv = NULL;
		if(argc > 1)
		{
			format = argv[1];
		}
		if(argc > 2)
		{
			extraArgc = argc-2;
			extraArgv = argv+2;
		}

		//если строка и есть такой атрибут в конфе - то взять его значение
		if(JSVAL_IS_STRING(format))
		{
			if(_confTimeFormats)
			{
				jsid id;
				if(!JS_ValueToId(ecx()->_jsCx, format, &id))return false;
				JSBool found;
				if(JS_HasPropertyById(ecx()->_jsCx, _confTimeFormats, id, &found) && found)
				{
					if(!JS_GetPropertyById(ecx()->_jsCx, _confTimeFormats, id, &format))return false;
				}
			}
		}

		//если строка - то трактовать ее как формат
		if(JSVAL_IS_STRING(format))
		{
			char *strFormat = NULL;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &format, "s", &strFormat))return false;

			//to milliseconds since epoch
			jsdouble dtime;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &time, "I", &dtime))return false;
			boost::posix_time::ptime ptime(boost::gregorian::date(1970, 1, 1), boost::posix_time::millisec((long long)(dtime+0.5)));
	
			std::string result = _timeFormatter.format(ptime, strFormat);
			*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, result.c_str(), result.size()));
			return true;
		}

		//если функция - то вызвать ее с дополнительными параметрами
		if(JSVAL_IS_OBJECT(format))
		{
			JSObject *formatObject = JSVAL_TO_OBJECT(format);
			if(JS_ObjectIsFunction(ecx()->_jsCx, formatObject))
			{
				if(extraArgc)
				{
					std::vector<jsval> fargv(extraArgc+1);
					fargv[0] = time;
					std::copy(extraArgv, extraArgv+extraArgc, fargv.begin()+1);
					return JS_CallFunctionValue(ecx()->_jsCx, _confTime, format, fargv.size(), &fargv[0], rval)?true:false;
				}
				else
				{
					return JS_CallFunctionValue(ecx()->_jsCx, _confTime, format, 1, &time, rval)?true:false;
				}
			}
			else
			{
				//не функция, игнорировать
			}
		}

		//ничего не прошло, привести time к строке
		JSString *str;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &time, "S", &str))return false;
		*rval = STRING_TO_JSVAL(str);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool L10n::xetter_conf(jsval *vp, bool isGet)
	{
		*vp = _conf;
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	void L10n::setupNumberFormatter(JSObject *conf)
	{
		char delim0 = '.';
		char delim3 = 0;
		bool zero0 = true;
		const char *nan = "NaN";
		const char *inf = "Infinity";
		NumberFormatter::TMFactors factors10;
		NumberFormatter::TMFactors factors2;

		JSContext *cx = ecx()->_jsCx;
		jsval jsv;
		char *s;
		JSBool b;

		////
		(JSExceptionReporter)JS_GetProperty(cx, _confNumber, "delim0", &jsv);
		if(!JSVAL_IS_VOID(jsv))
		{
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
			delim0 = s[0];
		}
		////
		(JSExceptionReporter)JS_GetProperty(cx, _confNumber, "delim3", &jsv);
		if(!JSVAL_IS_VOID(jsv))
		{
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
			delim3 = s[0];
		}
		////
		(JSExceptionReporter)JS_GetProperty(cx, _confNumber, "zero0", &jsv);
		if(!JSVAL_IS_VOID(jsv))
		{
			JS_ConvertArguments(cx, 1, &jsv, "b", &b);
			zero0 = b?true:false;
		}
		////
		(JSExceptionReporter)JS_GetProperty(cx, _confNumber, "factors10", &jsv);
		if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
		{
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			JSIdArray *arr = JS_Enumerate(cx, obj);
			for(jsint i(0); i<arr->length; i++)
			{
				(JSExceptionReporter)JS_GetPropertyById(cx, obj, arr->vector[i], &jsv);
				char *s = NULL;
				(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);

				(JSExceptionReporter)JS_IdToValue(cx, arr->vector[i], &jsv);
				int32 i32;
				(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "i", &i32);

				factors10[i32] = s;
			}
			JS_DestroyIdArray(ecx()->_jsCx, arr);
		}
		////
		(JSExceptionReporter)JS_GetProperty(cx, _confNumber, "factors2", &jsv);
		if(JSVAL_IS_OBJECT(jsv) && !JSVAL_IS_NULL(jsv))
		{
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			JSIdArray *arr = JS_Enumerate(cx, obj);
			for(jsint i(0); i<arr->length; i++)
			{
				(JSExceptionReporter)JS_GetPropertyById(cx, obj, arr->vector[i], &jsv);
				char *s = NULL;
				(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);

				(JSExceptionReporter)JS_IdToValue(cx, arr->vector[i], &jsv);
				int32 i32;
				(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "i", &i32);

				factors2[i32] = s;
			}
			JS_DestroyIdArray(ecx()->_jsCx, arr);
		}




		_numberFormatter.setup(
			delim0, delim3, zero0, 
			nan, inf,
			factors10, factors2);
	}

	//////////////////////////////////////////////////////////////////////////
	void L10n::setupTimeFormatter(JSObject *conf, const boost::local_time::tz_database &tzDatabase)
	{
		using namespace boost::assign;

		TimeFormatter::TVString	dowShort;
		TimeFormatter::TVString	dowLong;
		TimeFormatter::TVString	monthShort;
		TimeFormatter::TVString	monthLong;

		std::string	am;
		std::string	pm;
		std::string	AM;
		std::string	PM;

		JSContext *cx = ecx()->_jsCx;
		jsval jsv;
		char *s;
		JSBool b;

		////
		if(JS_HasProperty(cx, conf, "dowShort", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "dowShort", &jsv);
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			JSIdArray *arr = JS_Enumerate(cx, obj);
			for(jsint i(0); i<arr->length; i++)
			{
				(JSExceptionReporter)JS_GetPropertyById(cx, obj, arr->vector[i], &jsv);
				(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
				dowShort.push_back(s);
			}
			JS_DestroyIdArray(cx, arr);
		}
		else
		{
			dowShort += std::string("su"), "mo", "tu", "we", "th", "fr", "sa";
		}

		////
		if(JS_HasProperty(cx, conf, "dowLong", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "dowLong", &jsv);
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			JSIdArray *arr = JS_Enumerate(cx, obj);
			for(jsint i(0); i<arr->length; i++)
			{
				(JSExceptionReporter)JS_GetPropertyById(cx, obj, arr->vector[i], &jsv);
				(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
				dowLong.push_back(s);
			}
			JS_DestroyIdArray(cx, arr);
		}
		else
		{
			dowLong += std::string("sunday"), "monday", "tuesday", "wednesday", "thursday", "friday", "saturday";
		}

		////
		if(JS_HasProperty(cx, conf, "monthShort", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "monthShort", &jsv);
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			JSIdArray *arr = JS_Enumerate(cx, obj);
			for(jsint i(0); i<arr->length; i++)
			{
				(JSExceptionReporter)JS_GetPropertyById(cx, obj, arr->vector[i], &jsv);
				(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
				monthShort.push_back(s);
			}
			JS_DestroyIdArray(cx, arr);
		}
		else
		{
			monthShort += std::string("jan"), "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec";
		}

		////
		if(JS_HasProperty(cx, conf, "monthLong", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "monthLong", &jsv);
			JSObject *obj = JSVAL_TO_OBJECT(jsv);
			JSIdArray *arr = JS_Enumerate(cx, obj);
			for(jsint i(0); i<arr->length; i++)
			{
				(JSExceptionReporter)JS_GetPropertyById(cx, obj, arr->vector[i], &jsv);
				(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
				monthLong.push_back(s);
			}
			JS_DestroyIdArray(cx, arr);
		}
		else
		{
			monthLong += std::string("january"), "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december";
		}

		////
		if(JS_HasProperty(cx, conf, "am", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "am", &jsv);
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
			am = s;
		}
		else
		{
			am = "am";
		}

		////
		if(JS_HasProperty(cx, conf, "pm", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "pm", &jsv);
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
			pm = s;
		}
		else
		{
			pm = "pm";
		}

		////
		if(JS_HasProperty(cx, conf, "AM", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "AM", &jsv);
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
			AM = s;
		}
		else
		{
			AM = "AM";
		}

		////
		if(JS_HasProperty(cx, conf, "PM", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "PM", &jsv);
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
			PM = s;
		}
		else
		{
			PM = "PM";
		}

		boost::local_time::time_zone_ptr tz;

		////
		if(JS_HasProperty(cx, conf, "tz", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "tz", &jsv);
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);

			tz = tzDatabase.time_zone_from_region(s);

			if(!tz)
			{
				std::cerr<<"region for timezone is incorrect: "<<s<<std::endl;
				std::vector<std::string> regions = tzDatabase.region_list();
				for(size_t ri(0); ri<regions.size(); ri++)
				{
					boost::local_time::time_zone_ptr tzr = tzDatabase.time_zone_from_region(regions[ri]);

					if(tzr->dst_zone_abbrev() == s)
					{
						tz = tzr;
						std::cerr<<"use timezone: "<<regions[ri]<<" (dst name="<<s<<")"<<std::endl;
						break;
					}
					
					if(tzr->std_zone_abbrev() == s)
					{
						tz = tzr;
						std::cerr<<"use timezone: "<<regions[ri]<<" (std abbrev="<<s<<")"<<std::endl;
						break;
					}

					
					if(tzr->dst_zone_name() == s)
					{
						tz = tzr;
						std::cerr<<"use timezone: "<<regions[ri]<<" (dst name="<<s<<")"<<std::endl;
						break;
					}

					if(tzr->std_zone_name() == s)
					{
						tz = tzr;
						std::cerr<<"use timezone: "<<regions[ri]<<" (std name="<<s<<")"<<std::endl;
						break;
					}
				}
			}

		}

		_timeFormatter.setup(tz,
			dowShort, dowLong, 
			monthShort, monthLong, 
			am, pm, AM, PM);
	}

	//////////////////////////////////////////////////////////////////////////
	void L10n::setupTextFormatter(JSObject *conf)
	{
		JSContext *cx = ecx()->_jsCx;
		jsval jsv;
		JSBool b;

		////
		std::set<std::string> files;
		if(JS_HasProperty(cx, conf, "inDirectories", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "inDirectories", &jsv);
			filesFromDir(files, jsv);
		}
		if(JS_HasProperty(cx, conf, "inFiles", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "inFiles", &jsv);
			filesFrom(files, jsv);
		}

		std::string language;
		if(JS_HasProperty(cx, conf, "language", &b) && b)
		{
			(JSExceptionReporter)JS_GetProperty(cx, conf, "language", &jsv);
			char *s;
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &jsv, "s", &s);
			language = s;
		}
		_dtb = _i18n->mkDicTableBucket(files, language);
	}

	//////////////////////////////////////////////////////////////////////////
	void L10n::filesFromDir(std::set<std::string> &files, jsval conf)
	{
		std::set<std::string> dirs;
		filesFrom(dirs, conf, true);

		BOOST_FOREACH(const std::string &d, dirs)
		{
			boost::filesystem::directory_iterator diter(d);
			boost::filesystem::directory_iterator dend;
			for(; diter!=dend; diter++)
			{
				if(boost::filesystem::is_regular_file(*diter))
				{
					files.insert(diter->string());
				}
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////
	void L10n::filesFrom(std::set<std::string> &files, jsval conf, bool stubForDir)
	{
		JSContext *cx = ecx()->_jsCx;
		//array, string
		if(JSVAL_IS_OBJECT(conf))
		{
			if(JSVAL_IS_NULL(conf))
			{
				return;
			}

			JSObject *obj = JSVAL_TO_OBJECT(conf);

			JSIdArray *arr = JS_Enumerate(cx, obj);
			for(jsint i(0); i<arr->length; i++)
			{
				(JSExceptionReporter)JS_GetPropertyById(cx, obj, arr->vector[i], &conf);
				filesFrom(files, conf, stubForDir);
			}
			JS_DestroyIdArray(ecx()->_jsCx, arr);
		}
		else
		{
			char *s;
			(JSExceptionReporter)JS_ConvertArguments(cx, 1, &conf, "s", &s);

			std::string file = ecx()->_scripter->resolveLibName(s, NULL, stubForDir);

			if(!file.empty())
			{
				files.insert(file);
			}
			else
			{
				std::string curFile; int curLine;
				Scripter::evalCurPosition(&curFile, &curLine);
				std::cerr<<"unable to find translation file "<<s<<" from "<<curFile<<":"<<curLine<<std::endl;
			}
		}

	}

}
