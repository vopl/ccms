#ifndef _ccms_transportAsio_h_
#define _ccms_transportAsio_h_

#include "transport/transportBase.hpp"

#include "utils/deflate.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class TransportAsio;

	//////////////////////////////////////////////////////////////////////////
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> SocketPtr;
	typedef std::vector<char> DataBuf;
	typedef std::deque<char> DDataBuf;

	enum EConnectionState
	{
		ecsReadHeader,
		ecsReadBody,
		ecsWriteHeader,
		ecsWriteBody,
		ecsComplete,
	};

	enum EContentType
	{
		ectNull,
		ectFictive,
		ectFictiveNoProcess,
		ectFormData,
		ectFormUrlEncoded,
	};

	enum EFDState
	{
		efdsInit,
		efdsProcessHeader,
		efdsProcessBody,
		efdsComplete,
	};

	enum EContentEncoding
	{
		eceNone,
		eceDeflate,
		eceGzip,
	};

	struct Connection
		: Connection4Backend
	{
		TransportAsio		*_transport;

		EConnectionState	_state;
		SocketPtr			_socket;

		EContentType		_inContentType;
		std::string			_inBodyBoundary;

		EContentEncoding	_outContentEncoding;
		CompressorStreamPtr	_outCompressorStream;
		int					_outCompression;
		std::string			_chunkSize;

		size_t				_bytesTransfered;
		DDataBuf			_dataBuf;
		DataBuf				_netBuf;
		
		boost::shared_ptr<std::ifstream>	_staticFile;
		size_t								_staticFileSize;

		//для ectFormUrlEncoded акумуляторы
		bool		_fueWaitKey;
		std::string _fueAccumulerKey;
		std::string _fueAccumulerValue;

		//для ectFormData акумуляторы
		EFDState	_fdState;
		DataBuf		_fdBuffer;

		std::string							_fdName;
		std::string							_fdFileName;
		std::string							_fdContentType;
		boost::shared_ptr<std::ofstream>	_fdFile;
		std::string							_fdValue;

		Connection(
			TransportAsio *transport,
			SocketPtr socket, 
			std::ostream &err);

		~Connection();
	};
	typedef boost::shared_ptr<Connection> ConnectionPtr;

	//////////////////////////////////////////////////////////////////////////
	class TransportAsio
		: public TransportBase<ConnectionPtr>
	{
		boost::asio::io_service _io_service;
		boost::asio::ip::tcp::acceptor _acceptor;
		boost::asio::deadline_timer _cronTimer;

		typedef boost::shared_ptr<boost::asio::deadline_timer> TimerPtr;

		std::map<std::string, std::string> _extMimeTypes;
		std::map<std::string, int> _extCompress;

		unsigned long _keepaliveMax;

		unsigned long _headerbufGranula;
		unsigned long _headerbufLimit;
		unsigned long _bodybufGranula;
		unsigned long _bodybufLimit;

		unsigned long _outbufGranula;

		std::string _staticDirectory;

		time_t		_cronInterval;

		bool	_enableETag;
		bool	_enableLastModified;
		bool	_enableGzip;
		bool	_enableDeflate;



	public:
		TransportAsio(
			const char *host, 
			unsigned short port, 
			size_t queueSize,
			ITransportBackend *be);
		~TransportAsio();

		void set_mimeTypes(const std::map<std::string, std::string> &v){_extMimeTypes = v;};
		void set_compress(const std::map<std::string, int> &v){_extCompress = v;};
		void set_cronInterval(time_t v){_cronInterval = v;};

		void set_keepaliveMax(unsigned long v){_keepaliveMax = v;};

		void set_headerbufGranula(unsigned long v){_headerbufGranula = v;};
		void set_headerbufLimit(unsigned long v){_headerbufLimit = v;};
		void set_bodybufGranula(unsigned long v){_bodybufGranula = v;};
		void set_bodybufLimit(unsigned long v){_bodybufLimit = v;};

		void set_outbufGranula(unsigned long v){_outbufGranula = v;};
		void set_staticDirectory(const char *v){_staticDirectory = boost::filesystem::complete(v).normalize().string();};

		void set_enableETag(bool v){_enableETag = v;};
		void set_enableLastModified(bool v){_enableLastModified = v;};

		void set_enableGzip(bool v){_enableGzip = v;};
		void set_enableDeflate(bool v){_enableDeflate = v;};

		virtual bool start();
		virtual bool stop();

	private:
		void makeAccept();
		void handleAccept(SocketPtr socket, const boost::system::error_code& e);

		void makeRead(TimerPtr timer, ConnectionPtr connection, size_t granula);
		void handleRead(TimerPtr timer, ConnectionPtr connection, const boost::system::error_code& e, std::size_t bytes_transferred);

		void makeWrite(ConnectionPtr connection);
		void handleWrite(ConnectionPtr connection, const boost::system::error_code& e, std::size_t bytes_transferred);

		void handleKeepaliveTimeout(TimerPtr timer, ConnectionPtr connection, const boost::system::error_code& e);

		void handleCronTick(const boost::system::error_code& e);

	private:
		void processReadedHeader(ConnectionPtr connection, DDataBuf::iterator end);//full
		void processReadedBody(ConnectionPtr connection, std::size_t bytes_transferred);//partial
		void processWriteStatic(ConnectionPtr connection);

		void processCompletedConnection(ConnectionPtr connection);

	private:
		virtual void onFullQueue(ConnectionPtr connection);
		virtual void onCompleteProbe(ConnectionPtr connection, bool res);
		virtual void onCompleteProcess(ConnectionPtr connection, bool res);

		void onFullQueue_own(ConnectionPtr connection);
		void onCompleteProbe_own(ConnectionPtr connection, bool res);
		void onCompleteProcess_own(ConnectionPtr connection, bool res);

	public:
		virtual void onConectionDestroy(Connection *connection);

	private:
		void prepareHeaders(ConnectionPtr connection);
		std::string mimeTypeForExt(const std::string &ext);
		int compressForExt(const std::string &ext);

	private:

		bool parsePramsGet(ConnectionPtr connection, std::string::iterator begin, std::string::iterator end);
		bool parseCookies(ConnectionPtr connection, std::string::iterator begin, std::string::iterator end);

		bool readBodyInit(ConnectionPtr connection);
		bool readBodyStep(ConnectionPtr connection, size_t butesTransfered);
		bool readBodyStop(ConnectionPtr connection);

		template <class Iterator>
		bool readParamsStep(ConnectionPtr connection, Iterator begin, Iterator end);


		void readParamsStep_fdReset(ConnectionPtr connection);

		bool readParamsStep_fdHeader(ConnectionPtr connection, 
			DataBuf::iterator keyBegin, 
			DataBuf::iterator keyEnd,
			DataBuf::iterator valueBegin,
			DataBuf::iterator valueEnd);
		bool readParamsStep_fdData(ConnectionPtr connection, 
			DataBuf::iterator dataBegin, 
			DataBuf::iterator dataEnd,
			bool isLast);
	};
}
#endif
