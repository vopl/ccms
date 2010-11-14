#include "stdafx.h"
#include "db/sqlite3.hpp"

namespace ccms
{

	const char *sqlite3_resErrStr(int res)
	{
		switch(res)
		{
		case SQLITE_OK: return "Successful result";
		case SQLITE_ERROR: return "SQL error or missing database";
		case SQLITE_INTERNAL: return "Internal logic error in SQLite";
		case SQLITE_PERM: return "Access permission denied";
		case SQLITE_ABORT: return "Callback routine requested an abort";
		case SQLITE_BUSY: return "The database file is locked";
		case SQLITE_LOCKED: return "A table in the database is locked";
		case SQLITE_NOMEM: return "A malloc() failed";
		case SQLITE_READONLY: return "Attempt to write a readonly database";
		case SQLITE_INTERRUPT: return "Operation terminated by sqlite3_interrupt()";
		case SQLITE_IOERR: return "Some kind of disk I/O error occurred";
		case SQLITE_CORRUPT: return "The database disk image is malformed";
		case SQLITE_NOTFOUND: return "NOT USED. Table or record not found";
		case SQLITE_FULL: return "Insertion failed because database is full";
		case SQLITE_CANTOPEN: return "Unable to open the database file";
		case SQLITE_PROTOCOL: return "NOT USED. Database lock protocol error";
		case SQLITE_EMPTY: return "Database is empty";
		case SQLITE_SCHEMA: return "The database schema changed";
		case SQLITE_TOOBIG: return "String or BLOB exceeds size limit";
		case SQLITE_CONSTRAINT: return "Abort due to constraint violation";
		case SQLITE_MISMATCH: return "Data type mismatch";
		case SQLITE_MISUSE: return "Library used incorrectly";
		case SQLITE_NOLFS: return "Uses OS features not supported on host";
		case SQLITE_AUTH: return "Authorization denied";
		case SQLITE_FORMAT: return "Auxiliary database format error";
		case SQLITE_RANGE: return "2nd parameter to sqlite3_bind out of range";
		case SQLITE_NOTADB: return "File opened that is not a database file";
		case SQLITE_ROW: return "sqlite3_step() has another row ready";
		case SQLITE_DONE: return "sqlite3_step() has finished executing";
		}
		return "unknown error";
	}







	//////////////////////////////////////////////////////////////////////////
	Sqlite3Statement::Sqlite3Statement(Sqlite3Ptr db, sqlite3_stmt *handle)
		: JsObject(true, "Sqlite3Statement")
		, _db(db)
		, _handle(handle)
	{
		jsRegisterMeth("query", &Sqlite3Statement::call_query, 0);
		jsRegisterMeth("exec", &Sqlite3Statement::call_exec, 0);
	}

	//////////////////////////////////////////////////////////////////////////
	Sqlite3Statement::~Sqlite3Statement()
	{
		if(_handle)
		{
			sqlite3_finalize(_handle);
			_handle = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3Statement::bind(int idx, jsval val)
	{
		if(JSVAL_IS_INT(val))
		{
			sqlite3_bind_int(_handle, idx, JSVAL_TO_INT(val));
		}
		else if(JSVAL_IS_DOUBLE(val))
		{
			sqlite3_bind_double(_handle, idx, *JSVAL_TO_DOUBLE(val));
		}
		else if(JSVAL_IS_STRING(val) || JSVAL_IS_OBJECT(val) && !JSVAL_IS_NULL(val))
		{
			char *s;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &val, "s", &s))return false;
			sqlite3_bind_text(_handle, idx, s, strlen(s), SQLITE_TRANSIENT);
		}
		else
		{
			sqlite3_bind_null(_handle, idx);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3Statement::bind(uintN argc, jsval *argv, jsval *rval)
	{
		JSContext *cx = ecx()->_jsCx;

		//////////////////////////////////////////////////////////////////////////
		//bind
		bool done = false;
		if(1 == argc)
		{
			if(JSVAL_IS_OBJECT(argv[0]) && !JSVAL_IS_NULL(argv[0]))
			{
				JSObject *obj = JSVAL_TO_OBJECT(argv[0]);

				if(JS_IsArrayObject(cx, obj))
				{
					//по индексам
					jsuint length=0;
					if(!JS_GetArrayLength(cx, obj, &length))return false;
					for(jsuint idx(0); idx < length; idx++)
					{
						jsval v = JSVAL_VOID;
						if(!JS_GetElement(cx, obj, idx, &v))return false;
						if(!bind(idx+1, v))
						{
							return false;
						}
					}
					done = true;
				}
				else
				{
					//по именам
					char *s;
					JSIdArray *ids = JS_Enumerate(cx, obj);
					for(jsint i(0); i<ids->length; i++)
					{
						jsval idVal;

						if(	JS_IdToValue(cx, ids->vector[i], &idVal) &&
							JS_ConvertArguments(cx, 1, &idVal, "s", &s))
						{
							int idx = sqlite3_bind_parameter_index(_handle, s);
							if(idx)
							{
								jsval val;
								if(!JS_GetPropertyById(cx, obj, ids->vector[i], &val))return false;
								if(!bind(idx, val))
								{
									return false;
								}
							}
						}
					}
					JS_DestroyIdArray(cx, ids);
					done = true;
				}
			}
		}

		if(!done)
		{
			//по индексам
			for(uintN idx(0); idx < argc; idx++)
			{
				if(!bind(idx+1, argv[idx]))
				{
					return false;
				}
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3Statement::call_query(uintN argc, jsval *argv, jsval *rval)
	{
		std::vector<jsval> resArray;
		if(_handle)
		{
			if(!bind(argc, argv, rval))
			{
				sqlite3_clear_bindings(_handle);
				sqlite3_reset(_handle);
				return false;
			}


			//////////////////////////////////////////////////////////////////////////
			//exec and fetch
			int res = sqlite3_step(_handle);
			int columnCount = sqlite3_column_count(_handle);
			while(SQLITE_ROW == res)
			{
				size_t idx(resArray.size());
				JSObject *rec = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);

				for(int cidx(0); cidx<columnCount; cidx++)
				{
					jsval val = JSVAL_VOID;
					switch(sqlite3_column_type(_handle, cidx))
					{
					case SQLITE_INTEGER:
						val = INT_TO_JSVAL(sqlite3_column_int(_handle, cidx));
						break;
					case SQLITE_FLOAT:
						{
							double d = sqlite3_column_double(_handle, cidx);
							if(!JS_NewDoubleValue(ecx()->_jsCx, d, &val)) return false;

						}
						break;
					case SQLITE_NULL:
						val = JSVAL_VOID;
						break;
					case SQLITE_TEXT:
						{
							JSString *str = JS_NewStringCopyN(ecx()->_jsCx, (const char *)sqlite3_column_text(_handle, cidx), sqlite3_column_bytes(_handle, cidx));
							if(str)
							{
								val = STRING_TO_JSVAL(str);
							}
							//keep void
						}
						break;
					case SQLITE_BLOB:
						//val = unsupported;
						break;
					default:
						//val = unknown;
						break;
					}
					if(!JS_DefineProperty(
						ecx()->_jsCx, 
						rec, 
						sqlite3_column_name(_handle, cidx), 
						val, 
						NULL, NULL, JSPROP_ENUMERATE))return false;

				}

				resArray.push_back(OBJECT_TO_JSVAL(rec));
				res = sqlite3_step(_handle);
			}

			if(res && SQLITE_DONE != res)
			{
				JS_ReportError(ecx()->_jsCx, 
					"[Sqlite3Statement.exec %s, %s, %s]", 
					sqlite3_resErrStr(res), 
					sqlite3_errmsg(_db->getHandle()),
					sqlite3_sql(_handle));

				sqlite3_clear_bindings(_handle);
				sqlite3_reset(_handle);
				return false;
			}
		}
		sqlite3_clear_bindings(_handle);
		sqlite3_reset(_handle);

		if(resArray.size())
		{
			JSObject *arr = JS_NewArrayObject(ecx()->_jsCx, resArray.size(), &resArray[0]);
			*rval = OBJECT_TO_JSVAL(arr);
		}
		else
		{
			JSObject *arr = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);
			*rval = OBJECT_TO_JSVAL(arr);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3Statement::call_exec(uintN argc, jsval *argv, jsval *rval)
	{
		if(_handle)
		{
			if(!bind(argc, argv, rval))
			{
				sqlite3_clear_bindings(_handle);
				sqlite3_reset(_handle);
				return false;
			}

			//////////////////////////////////////////////////////////////////////////
			//exec
			int res = sqlite3_step(_handle);
			if(res && SQLITE_DONE != res && res != SQLITE_ROW)
			{
				JS_ReportError(ecx()->_jsCx, 
					"[Sqlite3Statement.exec %s, %s, %s]", 
					sqlite3_resErrStr(res), 
					sqlite3_errmsg(_db->getHandle()),
					sqlite3_sql(_handle));
				sqlite3_clear_bindings(_handle);
				sqlite3_reset(_handle);
				return false;
			}
			sqlite3_clear_bindings(_handle);
			sqlite3_reset(_handle);

			*rval = INT_TO_JSVAL(sqlite3_changes(_db->getHandle()));
		}
		else
		{
			*rval = INT_TO_JSVAL(0);
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	Sqlite3::Sqlite3()
		: JsObject(true, "Sqlite3")
		, _handle(NULL)
	{
		jsRegisterMeth("open", &Sqlite3::call_open, 1);
		jsRegisterMeth("close", &Sqlite3::call_close, 0);
		jsRegisterMeth("prepare", &Sqlite3::call_prepare, 1);
		jsRegisterMeth("query", &Sqlite3::call_query, 1);
		jsRegisterMeth("exec", &Sqlite3::call_exec, 1);

		jsRegisterProp("lastInstertRowid", &Sqlite3::xetter_lastInstertRowid, true);
	}

	//////////////////////////////////////////////////////////////////////////
	Sqlite3::~Sqlite3()
	{
		if(_handle)
		{
			sqlite3_close(_handle);
			_handle = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3::call_open(uintN argc, jsval *argv, jsval *rval)
	{
		if(_handle)
		{
			JS_ReportError(ecx()->_jsCx, "[Sqlite3.open already opened]");
			return false;
		}

		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Sqlite3.open must be called with 1 argument]");
			return false;
		}
		char *fname;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &fname))return false;

		int res = sqlite3_open(fname, &_handle);

		if(res)
		{
			JS_ReportError(ecx()->_jsCx, "[Sqlite3.open %s, %s]", sqlite3_resErrStr(res), fname);
			*rval = JSVAL_FALSE;
			return false;
		}

		//sqlite3_update_hook(_handle, &Sqlite3::update_hook, this);

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3::call_close(uintN argc, jsval *argv, jsval *rval)
	{
		if(!_handle)
		{
			JS_ReportError(ecx()->_jsCx, "[Sqlite3.close already closed]");
			return false;
		}
		sqlite3_close(_handle);
		_handle = NULL;
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3::call_prepare(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Sqlite3.prepare must be called with 1 argument]");
			return false;
		}

		char *sql;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &sql))return false;

		sqlite3_stmt *stmt_handle = NULL;
		int res = sqlite3_prepare_v2(_handle, sql, -1, &stmt_handle, NULL);
		if(res)
		{
			JS_ReportError(ecx()->_jsCx, 
				"[Sqlite3.prepare %s, %s, %s]", 
				sqlite3_resErrStr(res), 
				sqlite3_errmsg(_handle),
				sql);

			*rval = JSVAL_FALSE;
			return false;
		}

		Sqlite3StatementPtr stmt = mkp(new Sqlite3Statement(mkp(this, ROOTNAME), stmt_handle), ROOTNAME);
		*rval = stmt->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3::call_query(uintN argc, jsval *argv, jsval *rval)
	{
		if(!call_prepare(argc, argv, rval))
		{
			return false;
		}

		Sqlite3StatementPtr stmt = mkp<Sqlite3Statement>(JSVAL_TO_OBJECT(*rval), ROOTNAME);
		return stmt->call_query(argc-1, argv+1, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3::call_exec(uintN argc, jsval *argv, jsval *rval)
	{
		if(!call_prepare(argc, argv, rval))
		{
			return false;
		}

		Sqlite3StatementPtr stmt = mkp<Sqlite3Statement>(JSVAL_TO_OBJECT(*rval), ROOTNAME);
		return stmt->call_exec(argc-1, argv+1, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Sqlite3::xetter_lastInstertRowid(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			jsdouble d = (jsdouble)sqlite3_last_insert_rowid(_handle);
			return JS_NewDoubleValue(ecx()->_jsCx, d, vp)?true:false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	sqlite3 *Sqlite3::getHandle()
	{
		return _handle;
	}

	//////////////////////////////////////////////////////////////////////////
	void Sqlite3::update_hook(void *vp, int operation, char const *dbName, char const *tableName, sqlite3_int64 rowid)
	{
		Sqlite3 *this_ = (Sqlite3 *)vp;

		//operation 
		switch(operation)
		{
		case SQLITE_INSERT:
			std::cout<<"SQLITE_INSERT"<<std::endl;
			break;
		case SQLITE_DELETE:
			std::cout<<"SQLITE_DELETE"<<std::endl;
			break;
		case SQLITE_UPDATE:
			std::cout<<"SQLITE_UPDATE"<<std::endl;
			break;
		default:
			std::cout<<"WHAT?"<<std::endl;
		}
		//

	}

}

