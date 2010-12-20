#include "stdafx.h"
#include "transport/transportAsioSocket.hpp"

namespace ccms
{

	//////////////////////////////////////////////////////////////////////////
	TransportAsioSocket::TransportAsioSocket(boost::asio::io_service& io_service)
		: _isSsl(false)
	{
		_socket._raw = new TRawSocket(io_service);
	}

	//////////////////////////////////////////////////////////////////////////
	TransportAsioSocket::TransportAsioSocket(boost::asio::io_service& io_service, boost::asio::ssl::context& context)
		: _isSsl(true)
	{
		_socket._ssl = new TSslSocket(io_service, context);
	}
	
	//////////////////////////////////////////////////////////////////////////
	TransportAsioSocket::~TransportAsioSocket()
	{
		close();
		if(_isSsl)
			delete _socket._ssl;
		else
			delete _socket._raw;
	}

	//////////////////////////////////////////////////////////////////////////
	TransportAsioSocket::TRawSocket &TransportAsioSocket::getRaw()
	{
		assert(!_isSsl);
		return *_socket._raw;
	}
	
	//////////////////////////////////////////////////////////////////////////
	TransportAsioSocket::TSslSocket &TransportAsioSocket::getSsl()
	{
		assert(_isSsl);
		return *_socket._ssl;
	}

	//////////////////////////////////////////////////////////////////////////
	boost::asio::io_service& TransportAsioSocket::io_service()
	{
		if(_isSsl)
		{
			return _socket._ssl->io_service();
		}

		return _socket._raw->io_service();
	}

	//////////////////////////////////////////////////////////////////////////
	boost::asio::ip::tcp::endpoint TransportAsioSocket::remote_endpoint()
	{
		if(_isSsl)
		{
			return _socket._ssl->next_layer().remote_endpoint();
		}

		return _socket._raw->remote_endpoint();
	}

	//////////////////////////////////////////////////////////////////////////
	size_t TransportAsioSocket::in_avail()
	{
		if(_isSsl)
		{
			boost::system::error_code ec;
			size_t res = _socket._ssl->in_avail(ec);
			if(ec) return 0;
			return res;
		}

		boost::asio::ip::tcp::socket::bytes_readable command;

		boost::system::error_code ec;
		_socket._raw->io_control(command);
		if(ec)
		{
			return 0;
		}

		return command.get();
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsioSocket::close()
	{
		if(_isSsl)
		{
			boost::system::error_code ec;
			_socket._ssl->shutdown(ec);
// 			_socket._ssl->next_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
// 			_socket._ssl->next_layer().close(ec);
// 			_socket._ssl->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
// 			_socket._ssl->lowest_layer().close(ec);

// 			_socket._ssl->async_shutdown(
// 				boost::bind(
// 					&TransportAsioSocket::handleShutdownSsl, this->shared_from_this(),
// 					boost::asio::placeholders::error)
// 				);
		}
		else
		{
			boost::system::error_code ec;
			_socket._raw->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
			_socket._raw->close(ec);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsioSocket::handleShutdownSsl(const boost::system::error_code& e)
	{
		boost::system::error_code ec;
		_socket._ssl->next_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		_socket._ssl->next_layer().close(ec);
	}


}
