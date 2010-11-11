#include "stdafx.h"
#include "net/smtp.hpp"
#include "utils/base64.h"

namespace ccms{namespace net{

	//////////////////////////////////////////////////////////////////////////
	Smtp::Smtp()
		: JsObject(true, "[object Smtp]")
		, _io_service()
		, _resolver(_io_service)
		, _socket(_io_service)
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, 4, 
			OBJECT_TO_JSVAL(JS_NewArrayObject(ecx()->_jsCx, 0, NULL)));

		(JSExceptionReporter)jsRegisterProp("host", &Smtp::xetter_host, false);
		(JSExceptionReporter)jsRegisterProp("port", &Smtp::xetter_port, false);
		(JSExceptionReporter)jsRegisterProp("login", &Smtp::xetter_login, false);
		(JSExceptionReporter)jsRegisterProp("password", &Smtp::xetter_password, false);
		(JSExceptionReporter)jsRegisterProp("queue", &Smtp::xetter_queue, true);

		(JSExceptionReporter)jsRegisterMeth("flush", &Smtp::call_flush, 0);
		(JSExceptionReporter)jsRegisterMeth("send", &Smtp::call_send, 1);
	}
	//////////////////////////////////////////////////////////////////////////
	Smtp::~Smtp()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::xetter_host(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, 0, vp)?true:false;
		}
		else
		{
			return JS_SetReservedSlot(ecx()->_jsCx, _js, 0, *vp)?true:false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::xetter_port(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, 1, vp)?true:false;
		}
		else
		{
			return JS_SetReservedSlot(ecx()->_jsCx, _js, 1, *vp)?true:false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::xetter_login(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, 2, vp)?true:false;
		}
		else
		{
			return JS_SetReservedSlot(ecx()->_jsCx, _js, 2, *vp)?true:false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::xetter_password(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, 3, vp)?true:false;
		}
		else
		{
			return JS_SetReservedSlot(ecx()->_jsCx, _js, 3, *vp)?true:false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::xetter_queue(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			return JS_GetReservedSlot(ecx()->_jsCx, _js, 4, vp)?true:false;
		}
		else
		{
			//return JS_SetReservedSlot(ecx()->_jsCx, _js, 4, *vp)?true:false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::call_flush(uintN argc, jsval *argv, jsval *rval)
	{
		jsval jsv;
		if(!JS_GetReservedSlot(ecx()->_jsCx, _js, 4, &jsv)) return false;

		JSObject *arr = JSVAL_TO_OBJECT(jsv);
		if(!doSend(arr)) return false;

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::call_send(uintN argc, jsval *argv, jsval *rval)
	{
		JSObject *arr = JS_NewArrayObject(ecx()->_jsCx, argc, argv);
		if(!doSend(arr)) return false;

		*rval = JSVAL_VOID;
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Smtp::doSend(JSObject *arr)
	{
		jsuint amount;
		if(!JS_GetArrayLength(ecx()->_jsCx, arr, &amount)) return false;

		if(amount)
		{
			if(!doStart()) return false;

			for(jsuint i(0); i<amount; i++)
			{
				jsval jsv;
				if(!JS_GetElement(ecx()->_jsCx, arr, 0, &jsv))
				{
					doStop();
					return false;
				}

				if(JSVAL_IS_OBJECT(jsv))
				{

					mime::Part *mpart = dynamic_cast<mime::Part *>(JsObject::thisObj(JSVAL_TO_OBJECT(jsv)));
					if(mpart)
					{
						std::string from, to, data;
						//part.headers.From.addresses
						if(!mpart->getFromAll(from)) return false;
						//part.headers.To.addresses
						if(!mpart->getToAll(to)) return false;
						//part.value()
						if(!mpart->getData(data)) return false;

						if(!doMsg(from, to, data))
						{
							if(JS_IsExceptionPending(ecx()->_jsCx))
							{
								JS_ReportPendingException(ecx()->_jsCx);
								JS_ClearPendingException(ecx()->_jsCx);
							}
						}
					}
				}
				if(!JS_DeleteElement(ecx()->_jsCx, arr, 0))
				{
					doStop();
					return false;
				}
			}

			if(!doStop()) return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::doStart()
	{
		jsval jsvHost, jsvPort;
		if(!JS_GetReservedSlot(ecx()->_jsCx, _js, 0, &jsvHost)) return false;
		if(!JS_GetReservedSlot(ecx()->_jsCx, _js, 1, &jsvPort)) return false;

		const char *host, *port;
		if(JSVAL_IS_VOID(jsvHost) || JSVAL_IS_NULL(jsvHost))
		{
			host = "localhost";
		}
		else
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsvHost, "s", &host)) return false;
		}
		if(JSVAL_IS_VOID(jsvPort) || JSVAL_IS_NULL(jsvPort))
		{
			port = "25";
		}
		else
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsvPort, "s", &port)) return false;
		}



		try
		{
			boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), host, port);
			boost::asio::ip::tcp::resolver::iterator iterator = _resolver.resolve(query);

			_socket.connect(*iterator);
			//_socket.io_control(boost::asio::socket_base::non_blocking_io(true));
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "Smtp.send: %s", e.what());
			return false;
		}

		if(!read("220")) return false;
		if(!write("EHLO ccms\r\n")) return false;
		if(!read("250")) return false;








		jsval jsvLogin, jsvPassword;
		if(!JS_GetReservedSlot(ecx()->_jsCx, _js, 2, &jsvLogin)) return false;
		if(!JS_GetReservedSlot(ecx()->_jsCx, _js, 3, &jsvPassword)) return false;

		const char *login, *password;
		if(JSVAL_IS_VOID(jsvLogin) || JSVAL_IS_NULL(jsvLogin))
		{
			login = "";
		}
		else
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsvLogin, "s", &login)) return false;
		}
		if(JSVAL_IS_VOID(jsvPassword) || JSVAL_IS_NULL(jsvPassword))
		{
			password = "";
		}
		else
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsvPassword, "s", &password)) return false;
		}


		if(login[0])
		{
			if(!write("AUTH LOGIN\r\n")) return false;
			if(read("334"))
			{
				std::string login64 = base64_encode(login)+"\r\n";
				if(!write(login64.data())) return false;
				if(!read("334")) return false;

				std::string password64 = base64_encode(password)+"\r\n";;
				if(!write(password64.data())) return false;
				if(!read("235")) return false;
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::read(const char *code0, const char *code1, const char *code2)
	{
		char buf[4096];
		size_t reply_length;

		try
		{
			reply_length = _socket.receive(boost::asio::buffer(buf, 4095));
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "Smtp.send read: %s", e.what());
			return false;
		}

		if(code0 || code1 || code2)
		{
			buf[reply_length] = 0;

			bool res = false;

			if(!res && code0 && !strncmp(code0, buf, strlen(code0))) res = true;
			if(!res && code1 && !strncmp(code1, buf, strlen(code1))) res = true;
			if(!res && code2 && !strncmp(code2, buf, strlen(code2))) res = true;

			if(!res)
			{
				JS_ReportError(ecx()->_jsCx, "Smtp.send unexpected response: %s", buf);
				return false;
			}
		}
		

		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Smtp::write(const char *data)
	{
		try
		{
			_socket.send(boost::asio::buffer(data, strlen(data)));
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "Smtp.send write: %s", e.what());
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::doMsg(const std::string &from, const std::string &to, const std::string &data)
	{
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep(", ");

		tokenizer tokens(from, sep);

		for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
		{
			if(!write("MAIL FROM: <")) return false;
			if(!write(tok_iter->data())) return false;
			if(!write(">\r\n")) return false;
			if(!read("250")) return false;
		}

		tokens.assign(to, sep);
		for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
		{
			if(!write("RCPT TO: <")) return false;
			if(!write(tok_iter->data())) return false;
			if(!write(">\r\n")) return false;
			if(!read("250")) return false;
		}

		if(!write("DATA\r\n")) return false;
		if(!read("354")) return false;

		if(!write(data.data())) return false;
		if(!write("\r\n.\r\n")) return false;
		if(!read("250")) return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Smtp::doStop()
	{
		try
		{
			if(_socket.is_open())
			{
				write("QUIT\r\n");
				read();
				_socket.close();
			}
		}
		catch(...)
		{
			//JS_ReportError(ecx()->_jsCx, "Smtp.send: %s", e.what());
			//return false;
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	SmtpConstructor::SmtpConstructor()
		: JsObject(true, "[object SmtpConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool SmtpConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new Smtp)->thisJsval();
		return true;
	}
}}
