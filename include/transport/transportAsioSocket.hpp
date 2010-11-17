#ifndef _ccms_transportAsioSocket_h_
#define _ccms_transportAsioSocket_h_

namespace ccms 
{

	class TransportAsioSocket;
	typedef boost::shared_ptr<TransportAsioSocket> TransportAsioSocketPtr;

	//////////////////////////////////////////////////////////////////////////
	class TransportAsioSocket
		: public boost::enable_shared_from_this <TransportAsioSocket>
	{
		typedef boost::asio::ip::tcp::socket TRawSocket;
		typedef boost::asio::ssl::stream<TRawSocket> TSslSocket;

		bool _isSsl;
		union
		{
			TRawSocket	*_raw;
			TSslSocket	*_ssl;
		} _socket;

	public:
		TransportAsioSocket(boost::asio::io_service& io_service);
		TransportAsioSocket(boost::asio::io_service& io_service, boost::asio::ssl::context& context);
		~TransportAsioSocket();

		TRawSocket &getRaw();
		TSslSocket &getSsl();

		boost::asio::io_service& io_service();
		boost::asio::ip::tcp::endpoint remote_endpoint();
		size_t in_avail();

		void close();

		void handleShutdownSsl(const boost::system::error_code& e);


		template <class TBuffer, class TCallback>
 		void async_read(TBuffer buffer, TCallback callback);

		template <class TBuffer, class TCallback>
 		void async_write(TBuffer buffer, TCallback callback);
	};


	//////////////////////////////////////////////////////////////////////////
	template <class TBuffer, class TCallback>
	void TransportAsioSocket::async_read(TBuffer buffer, TCallback callback)
	{
		if(_isSsl)
			_socket._ssl->async_read_some(buffer, callback);
		else
			_socket._raw->async_read_some(buffer, callback);
	}

	template <class TBuffer, class TCallback>
	void TransportAsioSocket::async_write(TBuffer buffer, TCallback callback)
	{
		if(_isSsl)
			boost::asio::async_write(*_socket._ssl, buffer, callback);
		else
			boost::asio::async_write(*_socket._raw, buffer, callback);
	}


}

#endif
