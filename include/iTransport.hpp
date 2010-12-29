#ifndef _ccms_itransport_h_
#define _ccms_itransport_h_


namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	struct Connection4Backend;

	//////////////////////////////////////////////////////////////////////////
	struct ParamFile
	{
		std::string	_nameClient;
		std::string	_nameServer;
		std::string	_contentType;
		size_t	_size;
	};

	//////////////////////////////////////////////////////////////////////////
	typedef std::map<std::string, std::string> TEnvMap;

	typedef std::deque<std::string> TDStrings;
	typedef std::map<std::string, TDStrings> TMDStrings;

	typedef std::deque<ParamFile> TDFiles;
	typedef std::map<std::string, TDFiles> TMDFiles;

	//////////////////////////////////////////////////////////////////////////
	class ITransportBackend
	{
	public:
		virtual bool initializeThread() = 0;
		virtual void deinitializeThread() = 0;

		virtual bool probe(Connection4Backend *connection) = 0;
		virtual bool process(Connection4Backend *connection) = 0;
		virtual void cleanup(void *connectionData) = 0;
		virtual void cronTick(std::ostream &err) = 0;

		// comet
// 		virtual bool afterWaitEvent(Connection4Backend *connection, const char *event) = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	struct Connection4Backend
	{
		void			*_backendData;

		size_t			_inContentLength;

		time_t			_keepaliveTimeout;

		std::string		_protocolVersion;
		std::string		_requestPath;
		TEnvMap			_env;

		std::string		_staticPath;
		size_t			_outStatus;
		std::string		_outHeaders;
		std::string		_outBody;

		std::ostream &	_err;

		TMDStrings	_cookies;
		TMDStrings	_paramsGet;
		TMDStrings	_paramsPost;
		TMDFiles	_files;

		Connection4Backend(std::ostream &err)
			: _backendData(NULL)
			, _inContentLength(0)
			, _keepaliveTimeout(0)
			, _outStatus(0)
			, _err(err)
		{
		}

		virtual ~Connection4Backend()
		{
		}

	private:
		template<class T> Connection4Backend(T);
		template<class T> void operator=(T);
	};



	//////////////////////////////////////////////////////////////////////////
	class ITransport
	{
	public:
		virtual bool run() = 0;
		virtual bool stop() = 0;

		// comet
// 		virtual void waitEvent(Connection4Backend *connection, const char *event, time_t timeout);
// 		virtual void fireEvent(const char *event);
	};
}
#endif
