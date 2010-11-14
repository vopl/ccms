#ifndef _ccms_pg_h_
#define _ccms_pg_h_

#include "scripter/jsObject.hpp"
#include <libpq-fe.h>


namespace ccms
{
	class Pg;
	typedef JsPtr<Pg> PgPtr;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class PgStatement
		: public JsObject
	{
		std::string	_name;
		std::string	_sql;
		bool _verbose;
		PgPtr		_db;

	public: 
		PgStatement(PgPtr db, const char *name, const char *sql, bool verbose);
		~PgStatement();

		const char *getSql();

		//recArray query([params]);
		bool call_query(uintN argc, jsval *argv, jsval *rval);
		//num exec([params]);
		bool call_exec(uintN argc, jsval *argv, jsval *rval);

		bool call_describe(uintN argc, jsval *argv, jsval *rval);
		bool xetter_sql(jsval *vp, bool isGet);
	};
	//typedef JsPtr<PgStatement> PgStatementPtr;


	//////////////////////////////////////////////////////////////////////////
	class Pg
		: public JsObject
	{

	public:
		Pg();
		~Pg();

		//open(filename)
		bool call_open(uintN argc, jsval *argv, jsval *rval);

		//close
		bool call_close(uintN argc, jsval *argv, jsval *rval);

		//Sqlite3Statement prepare(sql);
		bool call_prepare(uintN argc, jsval *argv, jsval *rval);


		//recArray query(string sql, [params]);
		bool call_query(uintN argc, jsval *argv, jsval *rval);

		//num exec(string sql, [params]);
		bool call_exec(uintN argc, jsval *argv, jsval *rval);

		//escape (quote) strings
		bool call_q(uintN argc, jsval *argv, jsval *rval);

		bool xetter_verbose(jsval *vp, bool isGet);

		bool xetter_status(jsval *vp, bool isGet);
		bool xetter_transactionStatus(jsval *vp, bool isGet);
		bool xetter_protocolVersion(jsval *vp, bool isGet);
		bool xetter_serverVersion(jsval *vp, bool isGet);


		PGconn *getConn();

	public:
		void onStatementCreate(PgStatement *stmt);
		void onStatementDestroy(PgStatement *stmt);
	private:
		typedef std::map<std::string, PgStatement *> TMStatements;
		TMStatements _statements;

	private:
		PGconn *	_conn;
		bool		_verbose;
	};
}
#endif
