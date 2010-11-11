#ifndef _ccms_sqlite3_h_
#define _ccms_sqlite3_h_

#include "scripter/jsObject.hpp"
#include "sqlite3.h"


namespace ccms
{
	class Sqlite3;
	typedef JsPtr<Sqlite3> Sqlite3Ptr;
	//////////////////////////////////////////////////////////////////////////
	class Sqlite3Statement
		: public JsObject
	{
		sqlite3_stmt	*_handle;
		Sqlite3Ptr		_db;

	public: 
		Sqlite3Statement(Sqlite3Ptr db, sqlite3_stmt *handle);
		~Sqlite3Statement();

		//recArray query([params]);
		bool call_query(uintN argc, jsval *argv, jsval *rval);
		//num exec([params]);
		bool call_exec(uintN argc, jsval *argv, jsval *rval);

	private:
		bool bind(int idx, jsval val);
		bool bind(uintN argc, jsval *argv, jsval *rval);
	};
	typedef JsPtr<Sqlite3Statement> Sqlite3StatementPtr;

	//////////////////////////////////////////////////////////////////////////
	class Sqlite3
		: public JsObject
	{
		sqlite3 *_handle;

	public:
		Sqlite3();
		~Sqlite3();

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

		//
		bool xetter_lastInstertRowid(jsval *vp, bool isGet);


	public:
		sqlite3 *getHandle();
	private:
		static void update_hook(void *,int, char const *, char const *, sqlite3_int64);
	};
}
#endif
