#ifndef _ccms_net_smtp_hpp_
#define _ccms_net_smtp_hpp_

#include "scripter/jsObject.hpp"
#include "mime/part.hpp"

namespace ccms {namespace net
{
	//////////////////////////////////////////////////////////////////////////
	//слоты 
	//0-host
	//1-port
	//2-login 
	//3-password 
	//4-queue
	class Smtp
		: public JsObject
	{
	public:
		Smtp();
		~Smtp();

		bool xetter_host(jsval *vp, bool isGet);
		bool xetter_port(jsval *vp, bool isGet);
		bool xetter_login(jsval *vp, bool isGet);
		bool xetter_password(jsval *vp, bool isGet);
		bool xetter_queue(jsval *vp, bool isGet);

		bool call_flush(uintN argc, jsval *argv, jsval *rval);
		bool call_send(uintN argc, jsval *argv, jsval *rval);

	private:
		bool doSend(JSObject *arr);

		boost::asio::io_service _io_service;
		boost::asio::ip::tcp::resolver _resolver;
		boost::asio::ip::tcp::socket _socket;

		bool doStart();
		bool doMsg(const std::string &from, const std::string &to, const std::string &data);
		bool doStop();

		bool read(const char *code0=NULL, const char *code1=NULL, const char *code2=NULL);
		bool write(const char *data);
	};
	typedef JsPtr<Smtp> SmtpPtr;



	//////////////////////////////////////////////////////////////////////////
	class SmtpConstructor
		: public JsObject
	{
	public:
		SmtpConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
