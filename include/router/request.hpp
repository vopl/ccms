#ifndef _ccms_request_h_
#define _ccms_request_h_

#include "iTransport.hpp"
#include "router/path.hpp"
#include "scripter/scripter.hpp"
#include "scripter/jsObject.hpp"

#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
namespace io = boost::iostreams;

typedef boost::iostreams::stream< boost::iostreams::back_insert_device<std::string> > OutStream;



namespace ccms
{
	class Router;
	//////////////////////////////////////////////////////////////////////////
	class Request
		: public JsObject
	{
	private:
		bool putParam(JSObject *place, std::string name, jsval value);

	private:
		friend class Router;
		Request(Connection4Backend *connection);
		~Request();

		bool initForHeaders();
		bool initForBody();

	public:
		void setStatusCode(size_t code);
		void pushHeader(const std::string &name, const std::string &value);
		void setStaticPath(const std::string &path);

		OutStream out;
		std::ostream &err;

	private:
		Connection4Backend *_connection;

	private:
		bool call_setStatusCode(uintN argc, jsval *argv, jsval *rval);
		bool call_pushHeader(uintN argc, jsval *argv, jsval *rval);

	private:
		JSObject *makeParams(const TMDStrings &map, JSObject *add);
		JSObject *makeParams(const TMDFiles &map, JSObject *add);
		JSObject *makeParams(const TEnvMap &map);
	};


	typedef JsPtr<Request> RequestPtr;






}

#endif
