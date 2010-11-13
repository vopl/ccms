#include "stdafx.h"
#include "router/request.hpp"
//#include "router/router.hpp"
#include "utils/string.hpp"




namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	bool Request::putParam(JSObject *place, std::string name, jsval value)
	{
		size_t pos = 0;
		for(;;)
		{
			size_t pos = name.find_first_of('.');
			if(pos == std::string::npos)
			{
				break;
			}

			name[pos] = '\0';
			JSBool b;
			if(!JS_HasProperty(ecx()->_jsCx, place, name.data(), &b)) return false;
			if(b)
			{
				jsval jsv;
				if(!JS_GetProperty(ecx()->_jsCx, place, name.data(), &jsv)) return false;
				place = JSVAL_TO_OBJECT(jsv);
			}
			else
			{
				JSObject *newPlace = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
				if(!JS_DefineProperty(ecx()->_jsCx, place, name.data(), OBJECT_TO_JSVAL(newPlace), NULL, NULL, JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT))
				{
					return false;
				}
				place = newPlace;
			}

			name.erase(0, pos+1);
		}

		if(!JS_DefineProperty(ecx()->_jsCx, place, name.data(), value, NULL, NULL, JSPROP_ENUMERATE|JSPROP_READONLY|JSPROP_PERMANENT))
		{
			return false;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	Request::Request(Connection4Backend *connection)
		: JsObject(true, "[object Request]")
		, _connection(connection)
		, out(connection->_outBody)
		, err(connection->_err)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Request::~Request()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Request::initForHeaders()
	{
		//добавить методы, параметры
		if(!jsRegisterMeth("setStatusCode", &Request::call_setStatusCode, 1)) return false;
		if(!jsRegisterMeth("pushHeader", &Request::call_pushHeader, 2)) return false;

		if(!jsRegisterProp("path", _connection->_requestPath, true)) return false;


		JSObject *cookies = makeParams(_connection->_cookies, NULL);
		if(!cookies) return false;
		if(!jsRegisterProp("cookies", cookies, true)) return false;

		JSObject *params = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		if(!params) return false;

		JSObject *paramsGet = makeParams(_connection->_paramsGet, params);
		if(!paramsGet) return false;
		if(!jsRegisterProp("paramsGet", paramsGet, true)) return false;

		JSObject *env = makeParams(_connection->_env);
		if(!env) return false;
		if(!jsRegisterProp("env", env, true)) return false;

		if(!jsRegisterProp("params", params, true)) return false;

		return true;
	}
	

	//////////////////////////////////////////////////////////////////////////
	bool Request::initForBody()
	{
		JSObject *params = JSVAL_TO_OBJECT(getProperty("params"));
		

		JSObject *paramsPost = makeParams(_connection->_paramsPost, params);
		if(!paramsPost) return false;
		if(!jsRegisterProp("paramsPost", paramsPost, true)) return false;

		JSObject *files = makeParams(_connection->_files, params);
		if(!files) return false;
		if(!jsRegisterProp("files", files, true)) return false;


		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	void Request::setStatusCode(size_t code)
	{
		_connection->_outStatus = code;
	}
	
	//////////////////////////////////////////////////////////////////////////
	void Request::pushHeader(const std::string &name, const std::string &value)
	{
		_connection->_outHeaders += name;
		_connection->_outHeaders += ": ";
		_connection->_outHeaders += value;
		_connection->_outHeaders += "\r\n";
	}

	//////////////////////////////////////////////////////////////////////////
	void Request::setStaticPath(const std::string &path)
	{
		_connection->_staticPath = path;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Request::call_setStatusCode(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = JSVAL_VOID;
		if(argc != 1)
		{
			JS_ReportError(ecx()->_jsCx, "[request.setStatusCode must be called with 1 argument]");
			return false;
		}

		setStatusCode(JSVAL_TO_INT(argv[0]));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Request::call_pushHeader(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = JSVAL_VOID;
		if(2 == argc)
		{
			char *nam;
			char *val;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &nam, &val))return false;
			pushHeader(nam, val);
			return true;
		}

		JS_ReportError(ecx()->_jsCx, "[request.pushHeader must be called with 2 arguments]");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	JSObject *Request::makeParams(const TMDStrings &map, JSObject *add)
	{
		JSObject *res = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		if(!res) return NULL;

		TMDStrings::const_iterator iter = map.begin();
		TMDStrings::const_iterator end = map.end();

		for(; iter!=end; iter++)
		{
			if(iter->second.size())
			{
				const std::string &strValue = iter->second.front();
				jsval jsv = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, strValue.data(), strValue.size()));

				if(!putParam(res, iter->first.data(), jsv)) return false;
				if(add)
				{
					if(!putParam(add, iter->first.data(), jsv)) return false;
				}

// 				if(!JS_DefineProperty(ecx()->_jsCx, res, iter->first.data(), jsv, NULL, NULL, JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_READONLY)) return NULL;
// 				if(add)
// 				{
// 					if(!JS_DefineProperty(ecx()->_jsCx, add, iter->first.data(), jsv, NULL, NULL, JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_READONLY)) return NULL;
// 				}
			}
		}

		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	JSObject *Request::makeParams(const TMDFiles &map, JSObject *add)
	{
		JSObject *res = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		if(!res) return NULL;

		TMDFiles::const_iterator iter = map.begin();
		TMDFiles::const_iterator end = map.end();

		for(; iter!=end; iter++)
		{
			if(iter->second.size())
			{
				const ParamFile &value = iter->second.front();
				JSObject *fjsv = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);

				jsval jsv = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value._contentType.data(), value._contentType.size()));
				if(!JS_DefineProperty(ecx()->_jsCx, fjsv, "contentType", jsv, NULL, NULL, JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_READONLY)) return NULL;

				jsv = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value._nameClient.data(), value._nameClient.size()));
				if(!JS_DefineProperty(ecx()->_jsCx, fjsv, "nameClient", jsv, NULL, NULL, JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_READONLY)) return NULL;

				jsv = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value._nameServer.data(), value._nameServer.size()));
				if(!JS_DefineProperty(ecx()->_jsCx, fjsv, "nameServer", jsv, NULL, NULL, JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_READONLY)) return NULL;

				jsv = INT_TO_JSVAL(value._size);
				if(!JS_DefineProperty(ecx()->_jsCx, fjsv, "size", jsv, NULL, NULL, JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_READONLY)) return NULL;

				jsv = OBJECT_TO_JSVAL(fjsv);

				if(!putParam(res, iter->first.data(), jsv)) return false;
				if(add)
				{
					if(!putParam(add, iter->first.data(), jsv)) return false;
				}
// 				if(!JS_DefineProperty(ecx()->_jsCx, res, iter->first.data(), jsv, NULL, NULL, JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_READONLY)) return NULL;
// 				if(add)
// 				{
// 					if(!JS_DefineProperty(ecx()->_jsCx, add, iter->first.data(), jsv, NULL, NULL, JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_READONLY)) return NULL;
// 				}
			}
		}

		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	JSObject *Request::makeParams(const TEnvMap &map)
	{
		JSObject *res = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		if(!res) return NULL;

		TEnvMap::const_iterator iter = map.begin();
		TEnvMap::const_iterator end = map.end();

		for(; iter!=end; iter++)
		{
			const std::string &strValue = iter->second;
			jsval jsv = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, strValue.data(), strValue.size()));
			if(!JS_DefineProperty(ecx()->_jsCx, res, iter->first.data(), jsv, NULL, NULL, JSPROP_ENUMERATE|JSPROP_PERMANENT|JSPROP_READONLY)) return NULL;
		}

		return res;
	}

}
