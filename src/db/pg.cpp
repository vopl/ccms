#include "stdafx.h"
#include "db/pg.hpp"
#include "utils/ncvt.h"
#include "scripter/scripter.hpp"
#include "scripter/profiler.hpp"


namespace ccms
{


	namespace pg_impl
	{
		template <class T>
		inline void revert(const T &v)
		{
			unsigned char *data = (unsigned char *)&v;
			unsigned len = sizeof(T);
			unsigned len_2 = len/2;
			for(unsigned i=0; i<len_2; i++)
			{	
				std::swap(data[i], data[len - i - 1]);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		typedef std::vector<char const *> TVBufPtrs;
		typedef std::vector<int> TVBufLens;


		bool bind(jsval v, 
			TVBufPtrs &paramValuesPtrs,
			TVBufLens &paramValuesLens)
		{
			JSContext *cx = ecx()->_jsCx;
			switch(JS_TypeOfValue(cx, v))
			{
			case JSTYPE_VOID:                /* undefined */
			case JSTYPE_NULL:                /* null */
				paramValuesPtrs.push_back(NULL);
				paramValuesLens.push_back(0);
				//bind null
				break;
			case JSTYPE_OBJECT:              /* object */
			case JSTYPE_FUNCTION:            /* function */
				{
					JSObject *obj = JSVAL_TO_OBJECT(v);
					if(!obj)
					{
						paramValuesPtrs.push_back(NULL);
						paramValuesLens.push_back(0);
						//bind null
						break;
					}

					JSClass *cls = JS_GET_CLASS(cx, obj);

					if(!strcmp("Date", cls->name))
					{
						//YYYY-MM-DD hh:mm:ss+tz
						jsval jsvs[2];

						if(!JS_CallFunctionName(cx, obj, "getTime", 0, NULL, jsvs+0)) return false;
						//if(!JS_CallFunctionName(cx, obj, "getTimezoneOffset", 0, NULL, jsvs+1)) return false;

						//int offset;
						jsdouble d;
						//if(!JS_ConvertArguments(cx, 2, jsvs, "di", &d, &offset))return false;
						if(!JS_ConvertArguments(cx, 1, jsvs, "d", &d))return false;
						time_t t = (time_t)(d/1000);
						//t -= offset*60;

						tm *ptm = localtime(&t);

						char buf[64];

						if(ptm)
						{
							sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d%", 
								ptm->tm_year+1900,
								ptm->tm_mon+1,
								ptm->tm_mday,
								ptm->tm_hour,
								ptm->tm_min,
								ptm->tm_sec);
						}
						else
						{
							sprintf(buf, "1970-01-01 00:00:00");
						}

						char *s;
						v  = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, buf));
						if(!JS_ConvertArguments(cx, 1, &v, "s", &s))return false;
						paramValuesPtrs.push_back(s);
						paramValuesLens.push_back(strlen(s));
					}
					else
					{
						char *s;
						if(!JS_ConvertArguments(cx, 1, &v, "s", &s))return false;
						paramValuesPtrs.push_back(s);
						paramValuesLens.push_back(strlen(s));
					}
				}
				break;
			case JSTYPE_XML:                 /* xml object */
				if(!JS_CallFunctionName(cx, JSVAL_TO_OBJECT(v), "toXMLString", 0, NULL, &v))return false;
				// no break
			case JSTYPE_STRING:              /* string */
			case JSTYPE_NUMBER:              /* number */
			case JSTYPE_BOOLEAN:             /* boolean */
				{
					char *s;
					if(!JS_ConvertArguments(cx, 1, &v, "s", &s))return false;
					paramValuesPtrs.push_back(s);
					paramValuesLens.push_back(strlen(s));
				}
				break;
			case JSTYPE_LIMIT:
				//wtf?
				assert(0);
			default:
				assert(!"unknown jsval type for binding");
			}

			return true;
		}


		//вариант 1 - набор значений
		//вариант 2 - одна переменная массив с набором значений
		bool bind(uintN argc, jsval *argv, 
			int &nParams, 
			TVBufPtrs &paramValuesPtrs, 
			TVBufLens &paramValuesLens)
		{
			nParams = 0;
			paramValuesPtrs.clear();
			paramValuesLens.clear();

			if(!argc)
			{
				return true;
			}

			JSContext *cx = ecx()->_jsCx;
			bool done = false;
			if(1 == argc)
			{
				//вариант 2 - одна переменная массив с набором значений
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
							if(!bind(v,
								paramValuesPtrs, 
								paramValuesLens))
							{
								return false;
							}
							nParams++;
						}
						done = true;
					}
				}
			}

			if(!done)
			{
				//вариант 1 - набор значений
				for(uintN i(0); i<argc; i++)
				{
					if(!bind(argv[i],
						paramValuesPtrs, 
						paramValuesLens))
					{
						return false;
					}
					nParams++;
				}
			}

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		bool makeExecResult(PGresult *pgRes, jsval *rval)
		{
			char *s = PQcmdTuples(pgRes);
			if(!s[0])
			{
				*rval = JSVAL_TRUE;
			}
			else
			{
				//*rval = STRING_TO_JSVAL(JS_NewStringCopyZ(ecx()->_jsCx, s));
				*rval = INT_TO_JSVAL(_atoi(s));
			}

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		bool makeQueryResult(PGresult *res, jsval *rval)
		{
			JSContext *cx = ecx()->_jsCx;
			int ntuples = PQntuples(res);

			if(ntuples)
			{
				int nfields = PQnfields(res);

				std::vector<jsval> jsvv(ntuples, JSVAL_VOID);

				for(int ntuple(0); ntuple<ntuples; ntuple++)
				{
					JSObject *tuple = JS_NewObject(cx, NULL, NULL, NULL);

					for(int nfield(0); nfield<nfields; nfield++)
					{
						if(PQgetisnull(res, ntuple, nfield))
						{
							if(!JS_DefineProperty(cx, tuple, PQfname(res, nfield), JSVAL_NULL, NULL, NULL, JSPROP_ENUMERATE))return false;
							continue;
						}
						char * value = PQgetvalue(res, ntuple, nfield);
						int length = PQgetlength(res, ntuple, nfield);
						Oid oid = PQftype(res, nfield);

						jsval jsv;

						switch(oid)
						{
						case 1082: // date
							{
								int YYYY,
									MM,
									DD;

								if(3 == sscanf(value, "%d-%d-%d",
									&YYYY,
									&MM,
									&DD))
								{
									//make new Date
									static JSObject *Date = JSVAL_TO_OBJECT(ecx()->_scripter->f_exec("return new Date", ".js"));
									static JSClass *DateClass = JS_GetClass(Date);


									jsval jsvs[3] = {INT_TO_JSVAL(YYYY), INT_TO_JSVAL(MM-1), INT_TO_JSVAL(DD)};

									//jsv = OBJECT_TO_JSVAL(JS_New(cx, Date, 1, &jsv));
									jsv = OBJECT_TO_JSVAL(JS_ConstructObjectWithArguments(cx, DateClass, NULL, NULL, 3, jsvs));
									break;
								}
							}
							jsv = STRING_TO_JSVAL(JS_NewStringCopyN(cx, value, length));
							break;
						case 1114: // timestamp
						case 1184: // timestamptz
							{
								int YYYY,
									MM,
									DD,
									hh,
									mm,
									ss;

								if(6 == sscanf(value, "%d-%d-%d %d:%d:%d",
									&YYYY,
									&MM,
									&DD,
									&hh,
									&mm,
									&ss))
								{
									tm stm = {ss, mm, hh, DD, MM-1, YYYY-1900};
									time_t t  = mktime(&stm);


									jsdouble td = (jsdouble)t * 1000;

									//make new Date
									static JSObject *Date = JSVAL_TO_OBJECT(ecx()->_scripter->f_exec("return new Date", ".js"));

									static JSClass *DateClass = JS_GetClass(Date);


									if(!JS_NewDoubleValue(ecx()->_jsCx, td, &jsv)) return false;

									//jsv = OBJECT_TO_JSVAL(JS_New(cx, Date, 1, &jsv));
									jsv = OBJECT_TO_JSVAL(JS_ConstructObjectWithArguments(cx, DateClass, NULL, NULL, 1, &jsv));
									break;
								}
							}
							jsv = STRING_TO_JSVAL(JS_NewStringCopyN(cx, value, length));
							break;

						case 16:   // bool
							if('t' == value[0])
							{
								jsv = JSVAL_TRUE;
							}
							else
							{
								jsv = JSVAL_FALSE;
							}
							break;

						case 700:  // float4
						case 701:  // float8
						case 1700: // numeric
							{
								if(!JS_NewDoubleValue(ecx()->_jsCx, atof(value), &jsv)) return false;
							}
							break;
						case 21:   // int2
							jsv = INT_TO_JSVAL(_atoi(value));
							break;
						case 23:   // int4
						case 26:   // oid
							{
								int i = _atoi(value);
								if(i < JSVAL_INT_MAX && i > JSVAL_INT_MIN)
								{
									jsv = INT_TO_JSVAL(i);
								}
								else
								{
									if(!JS_NewDoubleValue(ecx()->_jsCx, atof(value), &jsv)) return false;
									//jsv = DOUBLE_TO_JSVAL(&cd.x);
								}
							}
							break;
						case 20:   // int8
							{
								long long ll = _atoll(value);
								if(ll < JSVAL_INT_MAX && ll > JSVAL_INT_MIN)
								{
									jsv = INT_TO_JSVAL((int)ll);
								}
								else
								{
									if(!JS_NewDoubleValue(ecx()->_jsCx, atof(value), &jsv)) return false;
								}
							}
							break;
						default:
							jsv = STRING_TO_JSVAL(JS_NewStringCopyN(cx, value, length));
							break;
						}

						if(!JS_DefineProperty(cx, tuple, PQfname(res, nfield), jsv, NULL, NULL, JSPROP_ENUMERATE))return false;
					}

					jsvv[ntuple] = OBJECT_TO_JSVAL(tuple);

				}

				*rval = OBJECT_TO_JSVAL(JS_NewArrayObject(cx, ntuples, &jsvv[0]));
			}
			else
			{
				*rval = OBJECT_TO_JSVAL(JS_NewArrayObject(cx, 0, NULL));
			}

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		jsval oidTypeStr(Oid o)
		{
			JSContext *cx = ecx()->_jsCx;

			switch(o)
			{
			case 1082: // date
			case 1114: // timestamp
			case 1184: // timestamptz
				return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "datetime"));

			case 16:   // bool
				return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "bool"));

			case 700:  // float4
			case 701:  // float8
			case 1700: // numeric
				return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "real"));

			case 21:   // int2
			case 23:   // int4
				return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "int"));

			case 26:   // oid
				return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "oid"));

			case 20:   // int8
				return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "bigint"));

			case 1043:
				return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, "varchar"));
			}
			char tmp[64];
			sprintf(tmp, "oid=%d", o);
			return STRING_TO_JSVAL(JS_NewStringCopyZ(cx, tmp));
		}

		//////////////////////////////////////////////////////////////////////////
		bool makeDescribeResult(PGresult *res, jsval *rval)
		{
			/*
			{
				params:['int'],
				fields:[{name:'id', type:'int'}, {name:'myField', type:'varchar'}],
			}
			*/

			JSContext *cx = ecx()->_jsCx;

			std::vector<jsval> vparams(PQnparams(res));
			for(size_t i(0); i<vparams.size(); i++)
			{
				Oid ptype = PQparamtype(res, i);
				vparams[i] = oidTypeStr(ptype);
			}


			std::vector<jsval> vfields(PQnfields(res));
			for(size_t i(0); i<vfields.size(); i++)
			{
				JSObject *rec = JS_NewObject(cx, NULL, NULL, NULL);
				if(!rec) return false;
				if(!JS_DefineProperty(cx, rec, "name", STRING_TO_JSVAL(JS_NewStringCopyZ(cx, PQfname(res, i))), NULL, NULL, JSPROP_ENUMERATE)) return false;
				if(!JS_DefineProperty(cx, rec, "type", oidTypeStr(PQftype(res, i)), NULL, NULL, JSPROP_ENUMERATE)) return false;

				vfields[i] = OBJECT_TO_JSVAL(rec);
			}


			JSObject *params = JS_NewArrayObject(cx, vparams.size(), vparams.size()?&vparams[0]:NULL);
			if(!params) return false;
			JSObject *fields = JS_NewArrayObject(cx, vfields.size(), vfields.size()?&vfields[0]:NULL);
			if(!fields) return false;
			JSObject *rvalo = JS_NewObject(cx, NULL, NULL, NULL);
			if(!rvalo) return false;

			if(!JS_DefineProperty(cx, rvalo, "params", OBJECT_TO_JSVAL(params), NULL, NULL, JSPROP_ENUMERATE)) return false;
			if(!JS_DefineProperty(cx, rvalo, "fields", OBJECT_TO_JSVAL(fields), NULL, NULL, JSPROP_ENUMERATE)) return false;
			
			*rval = OBJECT_TO_JSVAL(rvalo);

			return true;
		}

		enum EEvalMode
		{
			eem_sql=0x1,
			eem_statement=0x2,

			eem_exec=0x10,
			eem_query=0x20,
		};
		//////////////////////////////////////////////////////////////////////////
		bool eval(PGconn *conn, int mode, const char *statement, const char *sql, uintN argc, jsval *argv, jsval *rval)
		{
			PGresult *res = NULL;
			assert(eem_sql & mode || eem_statement & mode);

			if(argc > 0)
			{
				int nParams;
				TVBufPtrs paramValuesPtrs;
				TVBufLens paramValuesLens;
				if(!bind(argc, argv, nParams, paramValuesPtrs, paramValuesLens))return false;

				if(eem_sql & mode)
				{
					if(nParams>0)
					{
						{
#if USE_PROFILER
							ProfilerScopeHelper psh(g_profiler, NULL, (std::string(sql)+"; [raw]").c_str());
#endif
							res = PQexecParams(conn, statement, nParams, NULL, &paramValuesPtrs[0], &paramValuesLens[0], NULL, 0);
						}
					}
					else
					{
						{
#if USE_PROFILER
							ProfilerScopeHelper psh(g_profiler, NULL, (std::string(sql)+"; [raw]").c_str());
#endif
							res = PQexecParams(conn, statement, 0, NULL, NULL, NULL, NULL, 0);
						}
					}
				}
				else
				{
					if(nParams>0)
					{
						{
#if USE_PROFILER
							ProfilerScopeHelper psh(g_profiler, NULL, (std::string(sql)+"; [prepared]").c_str());
#endif
							res = PQexecPrepared(conn, statement, nParams, &paramValuesPtrs[0], &paramValuesLens[0], NULL, 0);
						}
					}
					else
					{
						{
#if USE_PROFILER
							ProfilerScopeHelper psh(g_profiler, NULL, (std::string(sql)+"; [prepared]").c_str());
#endif
							res = PQexecPrepared(conn, statement, 0, NULL, NULL, NULL, 0);
						}
					}
				}
			}
			else
			{
				if(eem_sql & mode)
				{
					{
#if USE_PROFILER
						ProfilerScopeHelper psh(g_profiler, NULL, (std::string(sql)+"; [once]").c_str());
#endif
						res = PQexec(conn, statement);
					}
				}
				else
				{
					{
#if USE_PROFILER
						ProfilerScopeHelper psh(g_profiler, NULL, (std::string(sql)+"; [prepared]").c_str());
#endif
						res = PQexecPrepared(conn, statement, 0, NULL, NULL, NULL, 0);
					}
				}
			}

			switch(PQresultStatus(res))
			{
			case PGRES_EMPTY_QUERY:			/* empty query string was executed */
				*rval = JSVAL_NULL;
				break;
			case PGRES_COMMAND_OK:			/* a query command that doesn't return
											* anything was executed properly by the
											* backend */
			case PGRES_TUPLES_OK:			/* a query command that returns tuples was
											* executed properly by the backend,
											* PGresult contains the result tuples */

				{
					bool b = false;

					assert(eem_exec & mode || eem_query & mode);
					if(eem_exec & mode) b = makeExecResult(res, rval);
					else if(eem_query & mode) b = makeQueryResult(res, rval);

					PQclear(res);
					return b;
				}
				break;
			case PGRES_COPY_OUT:			/* Copy Out data transfer in progress */
			case PGRES_COPY_IN:				/* Copy In data transfer in progress */
			case PGRES_BAD_RESPONSE:		/* an unexpected response was recv'd from
											* the backend */
			case PGRES_NONFATAL_ERROR:		/* notice or warning message */
			case PGRES_FATAL_ERROR:			/* query failed */
			default:
				{
					char *s = PQresultErrorMessage(res);
					JS_ReportError(ecx()->_jsCx, "[Pg.eval failed: %s, [%s]]", s, sql?sql:statement);
				}
				PQclear(res);
				return false;
			}

			return true;
		}
	}

	using namespace pg_impl;


	//////////////////////////////////////////////////////////////////////////
	PgStatement::PgStatement(Pg *db, const char *sql)
		: JsObject(true, "PgStatement")
		, _db(db)
		, _sql(sql)
	{
		JS_SetReservedSlot(ecx()->_jsCx, _js, 0, _db->thisJsval());

		jsRegisterMeth("query", &PgStatement::call_query, 0);
		jsRegisterMeth("exec", &PgStatement::call_exec, 0);
		jsRegisterMeth("describe", &PgStatement::call_describe, 0);
		jsRegisterProp("sql", &PgStatement::xetter_sql, true);

		_db->onStatementCreate(this);
	}

	//////////////////////////////////////////////////////////////////////////
	PgStatement::~PgStatement()
	{
		if(_db)
		{
			_db->onStatementDestroy(this);
		}
		dropPrepared();
	}

	//////////////////////////////////////////////////////////////////////////
	void PgStatement::dropPrepared()
	{
		if(!_name.empty() && _db)
		{
			PQclear(PQexec(_db->getConn(), ("DEALLOCATE "+_name).c_str()));
			_name.clear();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void PgStatement::dropDb()
	{
		_db = NULL;
	}


	//////////////////////////////////////////////////////////////////////////
	const char *PgStatement::getSql()
	{
		return _sql.data();
	}


	//////////////////////////////////////////////////////////////////////////
	bool PgStatement::call_query(uintN argc, jsval *argv, jsval *rval)
	{
		if(!prepare() || !_db) return false;
		if(_db->getVerbose())
		{
			(*ecx()->_err)<<_sql<<std::endl;
		}
		return eval(_db->getConn(), eem_statement|eem_query, _name.data(), _sql.data(), argc, argv, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool PgStatement::call_exec(uintN argc, jsval *argv, jsval *rval)
	{
		if(!prepare() || !_db) return false;
		if(_db->getVerbose())
		{
			(*ecx()->_err)<<_sql<<std::endl;
		}
		return eval(_db->getConn(), eem_statement|eem_exec, _name.data(), _sql.data(), argc, argv, rval);
	}



	//////////////////////////////////////////////////////////////////////////
	bool PgStatement::call_describe(uintN argc, jsval *argv, jsval *rval)
	{
		if(!prepare() || !_db) return false;

		if(_db->getVerbose())
		{
			(*ecx()->_err)<<"describe "<<_sql<<std::endl;
		}

		PGresult *res = PQdescribePrepared(_db->getConn(), _name.data());


		switch(PQresultStatus(res))
		{
		case PGRES_EMPTY_QUERY:			/* empty query string was executed */
			*rval = JSVAL_TRUE;
			break;
		case PGRES_COMMAND_OK:			/* a query command that doesn't return
										* anything was executed properly by the
										* backend */
		case PGRES_TUPLES_OK:			/* a query command that returns tuples was
										* executed properly by the backend,
										* PGresult contains the result tuples */

			{
				bool b = makeDescribeResult(res, rval);
				PQclear(res);
				return b;
			}
			break;
		case PGRES_COPY_OUT:			/* Copy Out data transfer in progress */
		case PGRES_COPY_IN:				/* Copy In data transfer in progress */
		case PGRES_BAD_RESPONSE:		/* an unexpected response was recv'd from
										* the backend */
		case PGRES_NONFATAL_ERROR:		/* notice or warning message */
		case PGRES_FATAL_ERROR:			/* query failed */
		default:
			{
				char *s = PQresultErrorMessage(res);
				JS_ReportError(ecx()->_jsCx, "[Pg.describe failed: %s, [%s]]", s, _sql.data());
			}
			PQclear(res);
			return false;
		}
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	bool PgStatement::xetter_sql(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			JSString *str = JS_NewStringCopyN(ecx()->_jsCx, _sql.data(), _sql.size());
			if(!str) return false;
			*vp = STRING_TO_JSVAL(str);
		}
		else
		{
			//ignore
		}

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	bool PgStatement::prepare()
	{
		if(!_name.empty())
		{
			return true;
		}

		if(!_db)
		{
			return false;
		}

		char stmt[32];

		PGconn *_conn = _db->getConn();
		bool inTrans = false;
		switch(PQtransactionStatus(_conn))
		{
		case PQTRANS_ACTIVE:
		case PQTRANS_INTRANS:
		case PQTRANS_INERROR:
			inTrans = true;
		}

		for(;;)
		{
			if(inTrans) PQclear(PQexec(_conn, "SAVEPOINT internal_prepare_09275782567835"));

			sprintf(stmt, "ps_%08x", rand()*rand());
			PGresult *res = PQprepare(_conn, stmt, _sql.data(), 0, NULL);
			ExecStatusType est = PQresultStatus(res);
			if(PGRES_FATAL_ERROR == est)
			{
				if(inTrans) PQclear(PQexec(_conn, "ROLLBACK TO SAVEPOINT internal_prepare_09275782567835"));
				char *s4 = PQresultErrorField(res, PG_DIAG_SQLSTATE);
				if(s4 && !strcmp(s4, "42P05"))//DUPLICATE PREPARED STATEMENT
				{
					PQclear(res);
					continue;
				}
			}
			if(inTrans) PQclear(PQexec(_conn, "RELEASE SAVEPOINT internal_prepare_09275782567835"));
			if(PGRES_COMMAND_OK != est)
			{
				char *s = PQresultErrorMessage(res);

				JS_ReportError(ecx()->_jsCx, "[Pg.prepare failed: %s [%s]]", s, _sql.data());
				PQclear(res);
				return false;
			}
			//ok
			PQclear(res);
			break;
		};

		_name = stmt;
		return true;
	}





	//////////////////////////////////////////////////////////////////////////
	Pg::Pg()
		: JsObject(true, "Pg")
		, _conn(NULL)
		, _verbose(false)
	{
		jsRegisterMeth("open", &Pg::call_open, 1);
		jsRegisterMeth("reset", &Pg::call_reset, 0);
		jsRegisterMeth("close", &Pg::call_close, 0);
		jsRegisterMeth("prepare", &Pg::call_prepare, 1);
		jsRegisterMeth("query", &Pg::call_query, 1);
		jsRegisterMeth("exec", &Pg::call_exec, 1);
		jsRegisterMeth("q", &Pg::call_q, 1);

		jsRegisterProp("verbose", &Pg::xetter_verbose, false);

		jsRegisterProp("status", &Pg::xetter_status, true);
		jsRegisterProp("transactionStatus", &Pg::xetter_transactionStatus, true);
		jsRegisterProp("protocolVersion", &Pg::xetter_protocolVersion, true);
		jsRegisterProp("serverVersion", &Pg::xetter_serverVersion, true);

	}

	//////////////////////////////////////////////////////////////////////////
	Pg::~Pg()
	{
		jsval jsv;
		call_close(0,0,&jsv);

		TMStatements::iterator iter = _statements.begin();
		TMStatements::iterator end = _statements.end();
		for(; iter!=end; iter++)
		{
			iter->second->dropDb();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::call_open(uintN argc, jsval *argv, jsval *rval)
	{
		if(_conn)
		{
			JS_ReportError(ecx()->_jsCx, "[Pg.open already opened]");
			return false;
		}

		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Pg.open must be called with 1 argument]");
			return false;
		}
		char *dsn;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &dsn))return false;

		_conn = PQconnectdb(dsn);
		if(_conn == NULL)
		{
			JS_ReportError(ecx()->_jsCx, "[Pg.open unexpected error]");
			jsval jsv;
			call_close(0,0,&jsv);
			return false;
		}
		if(PQstatus(_conn) != CONNECTION_OK)
		{
			char *errMsg = PQerrorMessage(_conn);

			JS_ReportError(ecx()->_jsCx, "[Pg.open error: %s (%s)]", errMsg, dsn);
			jsval jsv;
			call_close(0,0,&jsv);
			return false;
		}
		int i = PQsetClientEncoding(_conn, "UTF8");
		//int i = PQsetClientEncoding(_conn, "LATIN1");

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::call_close(uintN argc, jsval *argv, jsval *rval)
	{
		if(_conn)
		{
			PQfinish(_conn);
			_conn = NULL;
		}

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::call_prepare(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Pg.prepare must be called with 1 argument]");
			return false;
		}

		char *sql;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &sql))return false;

		TMStatements::iterator siter = _statements.find(sql);
		if(_statements.end() != siter)
		{
			*rval = siter->second->thisJsval();
			return true;
		}

		//тут создать объект statement, зарядить его именем stmt и вернуть
		PgStatement *p = new PgStatement(this, sql);

		*rval = p->thisJsval();
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Pg::call_query(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Pg.query must be called with 1 argument]");
			return false;
		}

		char *sql;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &sql))return false;


		if(_verbose)
		{
			(*ecx()->_err)<<sql<<std::endl;
		}
		return eval(_conn, eem_sql|eem_query, sql, sql, argc-1, argv+1, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::call_exec(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[Pg.exec must be called with 1 argument]");
			return false;
		}

		char *sql;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &sql))return false;

		if(_verbose)
		{
			(*ecx()->_err)<<sql<<std::endl;
		}
		return eval(_conn, eem_sql|eem_exec, sql, sql, argc-1, argv+1, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::call_q(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "[Pg.q must be called with 1 argument]");
			return false;
		}

		char *value;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value))return false;
		size_t size = strlen(value);

		std::vector<char> escaped(size*4+10);
		int err;
		size_t esize = PQescapeStringConn(_conn, &escaped[0], value, escaped.size(), &err);
		if(err)
		{
			assert(0);
		}

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, &escaped[0], esize));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::xetter_verbose(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = _verbose?JS_TRUE:JS_FALSE;
		}
		else
		{
			JSBool b;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "b", &b)) return false;
			_verbose = b?true:false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::xetter_status(jsval *vp, bool isGet)
	{
		if(!isGet)
		{
			return true;
		}


		JSContext *cx = ecx()->_jsCx;

		ConnStatusType cs = PQstatus(_conn);
		switch(cs)
		{
		case CONNECTION_OK:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "OK"));
			break;
		case CONNECTION_BAD:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "BAD"));
			break;
		case CONNECTION_STARTED:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "STARTED"));
			break;
		case CONNECTION_MADE:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "MADE"));
			break;
		case CONNECTION_AWAITING_RESPONSE:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "AWAITING_RESPONSE"));
			break;
		case CONNECTION_AUTH_OK:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "AUTH_OK"));
			break;
		case CONNECTION_SETENV:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "SETENV"));
			break;
		case CONNECTION_SSL_STARTUP:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "SSL_STARTUP"));
			break;
		case CONNECTION_NEEDED:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "NEEDED"));
			break;
		default:
			{
				char tmp[256];
				sprintf(tmp, "unknown %d", (int)cs);
				*vp = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, tmp));
			}
			break;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::xetter_transactionStatus(jsval *vp, bool isGet)
	{
		if(!isGet)
		{
			return true;
		}


		JSContext *cx = ecx()->_jsCx;

		PGTransactionStatusType ts = PQtransactionStatus(_conn);
		switch(ts)
		{
		case PQTRANS_IDLE:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "IDLE"));
			break;
		case PQTRANS_ACTIVE:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "ACTIVE"));
			break;
		case PQTRANS_INTRANS:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "INTRANS"));
			break;
		case PQTRANS_INERROR:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "INERROR"));
			break;
		case PQTRANS_UNKNOWN:
			*vp = STRING_TO_JSVAL(JS_InternString(cx, "UNKNOWN"));
			break;
		default:
			{
				char tmp[256];
				sprintf(tmp, "unknown %d", (int)ts);
				*vp = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, tmp));
			}
			break;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::xetter_protocolVersion(jsval *vp, bool isGet)
	{
		if(!isGet)
		{
			return true;
		}


		JSContext *cx = ecx()->_jsCx;

		*vp = INT_TO_JSVAL(PQprotocolVersion(_conn));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::xetter_serverVersion(jsval *vp, bool isGet)
	{
		if(!isGet)
		{
			return true;
		}


		JSContext *cx = ecx()->_jsCx;

		*vp = INT_TO_JSVAL(PQserverVersion(_conn));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::call_reset(uintN argc, jsval *argv, jsval *rval)
	{
		BOOST_FOREACH(TMStatements::value_type &v, _statements)
		{
			v.second->dropPrepared();
		}
		
		PQreset(_conn);

		if(PQstatus(_conn) != CONNECTION_OK)
		{
			char *errMsg = PQerrorMessage(_conn);

			JS_ReportError(ecx()->_jsCx, "[Pg.reset error: %s]", errMsg);
			return false;
		}
		int i = PQsetClientEncoding(_conn, "UTF8");
		//int i = PQsetClientEncoding(_conn, "LATIN1");

		*rval = true;
		return true;

	}


	//////////////////////////////////////////////////////////////////////////
	PGconn *Pg::getConn()
	{
		return _conn;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Pg::getVerbose()
	{
		return _verbose;
	}


	//////////////////////////////////////////////////////////////////////////
	void Pg::onStatementCreate(PgStatement *stmt)
	{
		_statements[stmt->getSql()] = stmt;
	}

	//////////////////////////////////////////////////////////////////////////
	void Pg::onStatementDestroy(PgStatement *stmt)
	{
		_statements.erase(stmt->getSql());
	}


}
