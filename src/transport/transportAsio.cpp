#include "stdafx.h"
#include "transport/transportAsio.hpp"
#include "transportBase.inc.hpp"
#include "utils/string.hpp"
#include "tokenizer.hpp"
#include "utils/ncvt.h"
#include <sys/stat.h>
#include "utils/httpDate.hpp"
#include "utils/crc32.hpp"





namespace ccms
{
	using namespace boost::asio;

	//////////////////////////////////////////////////////////////////////////
	Connection::Connection(
		TransportAsio *transport,
		TransportAsioSocketPtr socket, 
		std::ostream &err)
		: _transport(transport)
		, _state(ecsReadHeader)
		, _socket(socket)
		, _inContentType(ectNull)
		, _outContentEncoding(eceNone)
		, _outCompression(-1)
		, _bytesTransfered(0)
		, _staticFileSize(0)
		, Connection4Backend(err)
	{
	}

	Connection::~Connection()
	{
		_transport->onConectionDestroy(this);
		BOOST_FOREACH(TMDFiles::value_type &v1, _files)
		{
			BOOST_FOREACH(TDFiles::value_type &v2, v1.second)
			{
				if(!v2._nameServer.empty())
				{
					remove(v2._nameServer.data());
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	TransportAsio::TransportAsio(const char *host, unsigned short port, unsigned short portSsl, size_t	queueSize, ITransportBackend *backend)
		: TransportBase<ConnectionPtr>(host, port, portSsl, queueSize, backend)
		, _io_service()
		, _contextSsl(_io_service, ssl::context::sslv23)
		, _acceptor(_io_service)
		, _acceptorSsl(_io_service)
		, _cronTimer(_io_service)
		, _keepaliveMax(5)
		, _headerbufGranula(0x1000)
		, _headerbufLimit(0x10000)
		, _bodybufGranula(0x10000)
		, _bodybufLimit(0x800000)
		, _outbufGranula(0x40000)
		, _cronInterval(60)
		, _enableLastModified(true)
		, _enableETag(true)
		, _enableGzip(true)
		, _enableDeflate(true)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	TransportAsio::~TransportAsio()
	{

	}


	//////////////////////////////////////////////////////////////////////////
	bool TransportAsio::start()
	{
		if(!TransportBase<ConnectionPtr>::start()) return false;


		try
		{

			if(_port)
			{
				ip::tcp::resolver resolver(_io_service);

				char port[32];
				sprintf(port, "%u", unsigned(_port));
				ip::tcp::resolver::query query(_host, port);
				ip::tcp::endpoint endpoint = *resolver.resolve(query);


				_commonEnv["SERVER_ADDR"] = endpoint.address().to_string();
				sprintf(port, "%u", unsigned(endpoint.port()));
				_commonEnv["SERVER_PORT"] = port;


				_acceptor.open(endpoint.protocol());
				_acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
				_acceptor.set_option(socket_base::enable_connection_aborted(true));
				_acceptor.bind(endpoint);
				_acceptor.listen();

				makeAccept();
			}

			if(_portSsl)
			{
				_contextSsl.set_options(
					ssl::context::default_workarounds
					| ssl::context::no_sslv2
					| ssl::context::single_dh_use);
				_contextSsl.set_password_callback(boost::bind(&TransportAsio::handleGetPasswordSsl, this));

				if(!_ssl_certificate.empty())
				{
					_contextSsl.use_certificate_chain_file(_ssl_certificate);
				}

				if(!_ssl_privateKey.empty())
				{
					_contextSsl.use_private_key_file(_ssl_privateKey, ssl::context::pem);
				}

				if(!_ssl_tmpdh.empty())
				{
					_contextSsl.use_tmp_dh_file(_ssl_tmpdh);
				}

				ip::tcp::resolver resolver(_io_service);

				char port[32];
				sprintf(port, "%u", unsigned(_portSsl));
				ip::tcp::resolver::query query(_host, port);
				ip::tcp::endpoint endpoint = *resolver.resolve(query);


				sprintf(port, "%u", unsigned(endpoint.port()));
				_commonEnv["SERVER_PORTSSL"] = port;


				_acceptorSsl.open(endpoint.protocol());
				_acceptorSsl.set_option(ip::tcp::acceptor::reuse_address(true));
				_acceptorSsl.set_option(socket_base::enable_connection_aborted(true));
				_acceptorSsl.bind(endpoint);
				_acceptorSsl.listen();

				makeAcceptSsl();
			}




			////////////////////// кроновый таймер запустится
			handleCronTick(boost::system::error_code());



			_io_service.run();

			//если стоп был до запуска то форсировать вновь
			//stop();
			if(!_stop)
			{
				handleStop();
			}
			_io_service.reset();

		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			stop();
			return false;
		}

// 		_acceptor.close();
// 		_acceptorSsl.close();
// 		_cronTimer.cancel();

		return true;
	}


	bool TransportAsio::stop()
	{
		makeStop();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::makeAccept()
	{
		TransportAsioSocketPtr socket(new TransportAsioSocket(_io_service));

		_acceptor.async_accept(socket->getRaw(),
			boost::bind(&TransportAsio::handleAccept, this,
			socket,
			placeholders::error));
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::handleAccept(TransportAsioSocketPtr socket, const boost::system::error_code& e)
	{
		//socket->io_control(socket_base::non_blocking_io(true));

		if(_stop) return;

		makeAccept();

		if(e)
		{
			std::cerr<<"TransportAsio::handleAccept: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			return;
		}

		ConnectionPtr connection(new Connection(this, socket, std::cerr));
		makeRead(TimerPtr(), connection, _headerbufGranula);
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::makeAcceptSsl()
	{
		TransportAsioSocketPtr socket(new TransportAsioSocket(_io_service, _contextSsl));

		_acceptorSsl.async_accept(socket->getSsl().lowest_layer(),
			boost::bind(&TransportAsio::handleAcceptSsl, this,
				socket,
				placeholders::error));
	}
	
	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::handleAcceptSsl(TransportAsioSocketPtr socket, const boost::system::error_code& e)
	{
		if(_stop) return;

		makeAcceptSsl();

		if(e)
		{
			std::cerr<<"TransportAsio::handleAcceptSsl: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			return;
		}

		ConnectionPtr connection(new Connection(this, socket, std::cerr));

		socket->getSsl().async_handshake(
			ssl::stream_base::server,
			boost::bind(
				&TransportAsio::handleHandshakeSsl, this,
				connection,
				placeholders::error));
	}

	//////////////////////////////////////////////////////////////////////////
	const std::string &TransportAsio::handleGetPasswordSsl()
	{
		return _ssl_password;
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::handleHandshakeSsl(ConnectionPtr connection, const boost::system::error_code& e)
	{
		if(e)
		{
			//std::cerr<<"TransportAsio::handleHandshakeSsl: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			connection->_socket->close();
			return;
		}

		connection->_env["SSL"] = "1";
		makeRead(TimerPtr(), connection, _headerbufGranula);
	}


	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::makeRead(TimerPtr timer, ConnectionPtr connection, size_t granula)
	{
		if(connection->_netBuf.size() < granula)
		{
			connection->_netBuf.resize(granula);
		}

		connection->_socket->async_read(
			buffer(
				&connection->_netBuf[0], 
				granula), 
			boost::bind(&TransportAsio::handleRead, this,
				timer,
				connection,
				placeholders::error,
				placeholders::bytes_transferred)
			);
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::handleRead(TimerPtr timer, ConnectionPtr connection, const boost::system::error_code& e, std::size_t bytes_transferred)
	{
		if(timer)
		{
			timer->cancel();
			timer.reset();
		}

		if(_stop) return;

		switch(e.value())
		{
		case 0:
			break;
		case error::operation_aborted:
			//std::cerr<<"TransportAsio::handleRead: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			{
				//boost::system::error_code ec;
				//connection->_socket->shutdown(ip::tcp::socket::shutdown_both, ec);
				connection->_socket->close();
			}
			return;
		case error::connection_aborted:
		case error::connection_refused:
		case error::connection_reset:
		case error::eof:
			//std::cerr<<"TransportAsio::handleRead: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			{
				//boost::system::error_code ec;
				//connection->_socket->shutdown(ip::tcp::socket::shutdown_both, ec);
				connection->_socket->close();
			}
			return;
		default:
			std::cerr<<"TransportAsio::handleRead: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			{
				//boost::system::error_code ec;
				//connection->_socket->shutdown(ip::tcp::socket::shutdown_both, ec);
				connection->_socket->close();
			}
			return;
		}

		switch(connection->_state)
		{
		case ecsReadHeader:
			{
				size_t lastDataBufProcessed = connection->_dataBuf.size();
				connection->_dataBuf.insert(
					connection->_dataBuf.end(), 
					connection->_netBuf.begin(),
					connection->_netBuf.begin()+bytes_transferred);

				//найти конец заголовка
				size_t lookStart = (lastDataBufProcessed>=3 ? lastDataBufProcessed-3 : 0);
				size_t lookStop = connection->_dataBuf.size();

				static const char CRLF2[] = "\r\n\r\n";

				DDataBuf::iterator lookBegin = connection->_dataBuf.begin()+lookStart;
				DDataBuf::iterator lookEnd = connection->_dataBuf.begin()+lookStop;
				DDataBuf::iterator crlf2Iter = std::search(lookBegin, lookEnd, CRLF2, CRLF2+4);

				if(lookEnd != crlf2Iter)
				{
					//заголовок найден
					processReadedHeader(connection, crlf2Iter+4);
					return;
				}

				//заголовок не завершен
				if(connection->_dataBuf.size() > _headerbufLimit)
				{
					//превышение лимита буфера - слишком большой заголовок
					std::cerr<<"TransportAsio::handleRead: header is too large"<<std::endl;
					return;
				}

				//читать дальше
				makeRead(TimerPtr(), connection, _headerbufGranula);
				return;
			}

			break;
		case ecsReadBody:
			processReadedBody(connection, bytes_transferred);
			break;
		default:
			assert(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::makeWrite(ConnectionPtr connection)
	{
		if(_stop) return;

		switch(connection->_state)
		{
		case ecsWriteHeader:
			if(connection->_bytesTransfered < connection->_outHeaders.size())
			{
				connection->_socket->async_write(
					buffer(
						&connection->_outHeaders[connection->_bytesTransfered], 
						connection->_outHeaders.size() - connection->_bytesTransfered), 
					boost::bind(&TransportAsio::handleWrite, this,
						connection,
						placeholders::error,
						placeholders::bytes_transferred)
					);
				return;
			}
			connection->_state = ecsWriteBody;
			connection->_bytesTransfered = 0;
			connection->_netBuf.clear();
			//break;
		case ecsWriteBody:

			if(connection->_staticFile)
			{
				if(connection->_outCompressorStream)
				{
					if(connection->_staticFileSize > connection->_outCompressorStream->inProcessed() || connection->_outCompressorStream->inAvail())
					{
						//наполнить буфер
						{
							bool finish = false;
							size_t spaceRead = _outbufGranula*2 - connection->_outCompressorStream->inAvail();
							size_t totalRead = spaceRead+connection->_outCompressorStream->inProcessed() + connection->_outCompressorStream->inAvail();
							if(totalRead >= connection->_staticFileSize)
							{
								finish = true;
								totalRead = connection->_staticFileSize;
								spaceRead = totalRead - connection->_outCompressorStream->inProcessed() - connection->_outCompressorStream->inAvail();
							}

							if(spaceRead)
							{
								char *newData = connection->_outCompressorStream->push(spaceRead, finish);
								connection->_staticFile->read(newData, spaceRead);
							}
						}

						//забрать пресованное
						{
							size_t inBuffer = connection->_netBuf.size();
							size_t spaceSend = _outbufGranula;

							//если в исходящем буфере меньше половины гранулы то добавить чанк
							if(inBuffer < _outbufGranula/2)
							{
								if(connection->_chunkSize.empty())
								{
									connection->_chunkSize = _htoa(_outbufGranula);
								}

								connection->_netBuf.resize(spaceSend + connection->_chunkSize.size() + 2 + 2 + 5);
								size_t spaceRead = spaceSend - inBuffer;
								bool finish;
								connection->_outCompressorStream->compress(&connection->_netBuf[inBuffer+connection->_chunkSize.size()+2], spaceRead, finish);

								std::string newChunkSize = _htoa(spaceRead);

								if(newChunkSize.size() > connection->_chunkSize.size())
								{
									//двинуть вперед
									connection->_netBuf.insert(
										connection->_netBuf.begin()+inBuffer,
										newChunkSize.begin(), 
										newChunkSize.begin()+ (newChunkSize.size() - connection->_chunkSize.size())
										);
								}
								else if(newChunkSize.size() < connection->_chunkSize.size())
								{
									//двинуть назад
									connection->_netBuf.erase(
										connection->_netBuf.begin()+inBuffer,
										connection->_netBuf.begin()+inBuffer + (connection->_chunkSize.size() - newChunkSize.size()));
								}
								else
								{
									//никуда не двигать
								}

								//положить размер чанка
								std::copy(
									newChunkSize.begin(),
									newChunkSize.end(),
									connection->_netBuf.begin()+inBuffer);

								//положить перевод строки между заголовком и данными
								const char CRLF[] = "\r\n";
								std::copy(
									CRLF,
									CRLF+2,
									connection->_netBuf.begin()+inBuffer+newChunkSize.size());

								//положить перевод строки после данных
								std::copy(
									CRLF,
									CRLF+2,
									connection->_netBuf.begin()+inBuffer+newChunkSize.size()+2+spaceRead);

								connection->_netBuf.resize(inBuffer + newChunkSize.size()+2+spaceRead+2);
								
								if(finish)
								{
									const char TERMINATOR[] = "0\r\n\r\n";
									connection->_netBuf.insert(connection->_netBuf.end(),
										TERMINATOR,
										TERMINATOR+5);
								}

								connection->_chunkSize = _htoa(_outbufGranula);
							}
						}
					}
				}
				else
				{
					if(connection->_bytesTransfered < connection->_staticFileSize)
					{
						size_t inBuffer = connection->_netBuf.size();
						size_t spaceSend = std::min((size_t)_outbufGranula, connection->_staticFileSize - connection->_bytesTransfered);
						if(inBuffer < spaceSend)
						{

							assert(spaceSend >= inBuffer);
							size_t spaceRead = spaceSend - inBuffer;

							connection->_netBuf.resize(spaceSend);
							if(spaceRead)
							{
								connection->_staticFile->read(&connection->_netBuf[inBuffer], spaceRead);
								//spaceRead = connection->_staticFile->gcount();
								//spaceSend = connection->_netBuf.size();
							}
						}
					}
				}


				if(!connection->_netBuf.empty())
				{
					connection->_socket->async_write(
						buffer(
							&connection->_netBuf[0], 
							connection->_netBuf.size()), 
						boost::bind(&TransportAsio::handleWrite, this,
							connection,
							placeholders::error,
							placeholders::bytes_transferred)
						);
					return;
				}

				connection->_netBuf.clear();
				connection->_staticFile.reset();
			}
			else
			{
				if(connection->_bytesTransfered < connection->_outBody.size())
				{
					connection->_socket->async_write(
						buffer(
							&connection->_outBody[connection->_bytesTransfered], 
							connection->_outBody.size() - connection->_bytesTransfered), 
						boost::bind(&TransportAsio::handleWrite, this,
							connection,
							placeholders::error,
							placeholders::bytes_transferred)
						);
					return;
				}
			}
			connection->_state = ecsComplete;
			processCompletedConnection(connection);
			break;
		default:
			assert(0);
		}

	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::handleWrite(ConnectionPtr connection, const boost::system::error_code& e, std::size_t bytes_transferred)
	{
		if(_stop) return;

		switch(e.value())
		{
		case 0:
			break;
		case error::operation_aborted:
		case error::connection_aborted:
		case error::connection_refused:
		case error::connection_reset:
		case error::broken_pipe:
			//std::cerr<<"TransportAsio::handleWrite: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			{
				//boost::system::error_code ec;
				//connection->_socket->shutdown(ip::tcp::socket::shutdown_both, ec);
				connection->_socket->close();
			}
			return;
		default:
			std::cerr<<"TransportAsio::handleWrite: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			{
				//boost::system::error_code ec;
				//connection->_socket->shutdown(ip::tcp::socket::shutdown_both, ec);
				connection->_socket->close();
			}
			return;
		}

		connection->_bytesTransfered += bytes_transferred;
		switch(connection->_state)
		{
		case ecsWriteHeader:
			makeWrite(connection);
			break;
		case ecsWriteBody:
			if(connection->_staticFile)
			{
				connection->_netBuf.erase(connection->_netBuf.begin(), connection->_netBuf.begin()+bytes_transferred);
			}
			makeWrite(connection);
			break;
		default:
			assert(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::handleKeepaliveTimeout(TimerPtr timer, ConnectionPtr connection, const boost::system::error_code& e)
	{
		timer->cancel();
		timer.reset();

		if(_stop) return;
		if(error::operation_aborted == e)
		{
			return;
		}

		if(e) 
		{ 
			std::cerr<<"TransportAsio::handleKeepaliveTimeout: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			return; 
		}

		//std::cout<<"TransportAsio::handleKeepaliveTimeout, "<<GetCurrentThreadId()<<", "<<connection->_socket->native()<<std::endl;
		//connection->_socket->cancel();
		//boost::system::error_code ec;
		//connection->_socket->shutdown(ip::tcp::socket::shutdown_both, ec);
		connection->_socket->close();
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::handleCronTick(const boost::system::error_code& e)
	{
		if(e) 
		{ 
			std::cerr<<"TransportAsio::handleCronTick: "<<e.message()<<"("<<e.value()<<")"<<std::endl;
			return; 
		}

		pushCronTick();

		boost::system::error_code ec;
		_cronTimer.expires_from_now(
			boost::posix_time::seconds((long)_cronInterval),
			ec); 
		if(ec)
		{
			std::cerr<<"TransportAsio::handleCronTick cronTimer.expires_from_now failed"<<ec.message()<<"("<<ec.value()<<")"<<std::endl;
		}
		else
		{
			_cronTimer.async_wait(
				boost::bind(&TransportAsio::handleCronTick, this,
					placeholders::error));
		}

	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::makeStop()
	{
		_io_service.post(
			boost::bind(&TransportAsio::handleStop, this));
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::handleStop()
	{
		TransportBase<ConnectionPtr>::stop();

 		boost::system::error_code ec;
 
 		_acceptor.cancel(ec);
 		_acceptor.close(ec);
 		_acceptorSsl.cancel(ec);
 		_acceptorSsl.close(ec);
// 		_cronTimer.cancel(ec);
 
		if(has_service<ssl::context_service>(_io_service))
		{
			use_service<ssl::context_service>(_io_service).shutdown_service();
		}

		if(has_service<deadline_timer_service<boost::posix_time::ptime> >(_io_service))
		{
			use_service<deadline_timer_service<boost::posix_time::ptime> >(_io_service).shutdown_service();
		}

		if(has_service<raw_socket_service<ip::tcp> >(_io_service))
		{
			use_service<raw_socket_service<ip::tcp> >(_io_service).shutdown_service();
		}

		if(has_service<socket_acceptor_service<ip::tcp> >(_io_service))
		{
			use_service<socket_acceptor_service<ip::tcp> >(_io_service).shutdown_service();
		}

		if(has_service<stream_socket_service<ip::tcp> >(_io_service))
		{
			use_service<stream_socket_service<ip::tcp> >(_io_service).shutdown_service();
		}

		_io_service.stop();
	}



	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::processReadedHeader(ConnectionPtr connection, DDataBuf::iterator end)
	{
		DDataBuf::iterator begin = connection->_dataBuf.begin();

		//распарсить заголовки в env
		TEnvMap &env = connection->_env;
		env = _commonEnv;

		static const char DOT2SP[] = ": ";
		static const char SP[] = " ";

		tokenizer<DDataBuf> lines(begin, end, "\r\n");
		tokenizer<DDataBuf>::iterator tok_iter = lines.begin();
		tokenizer<DDataBuf>::iterator tok_end = lines.end();

		if(tok_iter == tok_end)
		{
			std::cerr<<"TransportAsio::processReadedHeader: empty header"<<std::endl;
			return;
		}
		const IteratorPair<DDataBuf::iterator> &line = *tok_iter;
		DDataBuf::iterator i1 = std::find(line._begin, line._end, ' ');
		if(i1 == line._end)
		{
			std::cerr<<"TransportAsio::processReadedHeader: unrecognized header\n"<<std::string(line._begin, line._end)<<std::endl;
			return;
		}
		if(!trim(dropInvalidUtf8(env["REQUEST_METHOD"].assign(line._begin, i1))))
		{
			std::cerr<<"TransportAsio::processReadedHeader: unrecognized header\n"<<std::string(line._begin, line._end)<<std::endl;
			return;
		}
		
		if(i1 == line._end)
		{
			std::cerr<<"TransportAsio::processReadedHeader: unrecognized header\n"<<std::string(line._begin, line._end)<<std::endl;
			return;
		}
		i1++;

		static const char HTTP[] = "HTTP/";
		DDataBuf::iterator i2 = std::search(i1, line._end, HTTP, HTTP+5);
		if(i2 == line._end || i2 == i1)
		{
			std::cerr<<"TransportAsio::processReadedHeader: unrecognized header\n"<<std::string(line._begin, line._end)<<std::endl;
			return;
		}
		trim(dropInvalidUtf8(connection->_protocolVersion.assign(i2+5, line._end)));

		if(!trim(dropInvalidUtf8(connection->_requestPath.assign(i1, i2-1))))
		{
			std::cerr<<"TransportAsio::processReadedHeader: unrecognized header\n"<<std::string(line._begin, line._end)<<std::endl;
			return;
		}
		else
		{
			env["REQUEST_URI"] = connection->_requestPath;
			size_t pos = connection->_requestPath.find('?');
			if(pos != std::string::npos)
			{
				if(!parsePramsGet(connection, connection->_requestPath.begin()+pos+1, connection->_requestPath.end()))
				{
					std::cerr<<"TransportAsio::processReadedHeader: unrecognized get params"<<std::endl;
					return;
				}

				connection->_requestPath.erase(pos);
			}
			dropInvalidUtf8(hexdecode(connection->_requestPath));
		}

		static const char SPACES[] = " \t";
		DDataBuf::iterator i3 = std::find_first_of(i2, line._end, SPACES, SPACES+2);
		if(!trim(dropInvalidUtf8(env["SERVER_PROTOCOL"].assign(i2+5, i3))))
		{
			std::cerr<<"TransportAsio::processReadedHeader: unrecognized header\n"<<std::string(line._begin, line._end)<<std::endl;
			return;
		}

		tok_iter++;

		for(; tok_iter != tok_end; tok_iter++)
		{
			const IteratorPair<DDataBuf::iterator> &line = *tok_iter;
			DDataBuf::iterator delim = std::search(line._begin, line._end, DOT2SP, DOT2SP+2);
			if(delim == line._end)
			{
				std::cerr<<"TransportAsio::processReadedHeader: unrecognized header\n"<<std::string(line._begin, line._end)<<std::endl;
				return;
			}

			std::string key(line._begin, delim);
			if(!trim(dropInvalidUtf8(key)))
			{
				std::cerr<<"TransportAsio::processReadedHeader: unrecognized header\n"<<std::string(line._begin, line._end)<<std::endl;
				return;
			}
			tolowerLatin(key);
			std::string &value = env[key];
			value.assign(delim+2, line._end);
			if(!trim(dropInvalidUtf8(value)))
			{
				std::cerr<<"TransportAsio::processReadedHeader: unrecognized header\n"<<std::string(line._begin, line._end)<<std::endl;
				return;
			}

			if(key == "cookie")
			{
				if(!parseCookies(connection, value.begin(), value.end()))
				{
					std::cerr<<"TransportAsio::processReadedHeader: unrecognized cookies\n"<<std::string(line._begin, line._end)<<std::endl;
					return;
				}

			}

		}

		ip::tcp::endpoint rend = connection->_socket->remote_endpoint();
		env["REMOTE_ADDR"] = rend.address().to_string();
		char port[32];
		sprintf(port, "%u", unsigned(rend.port()));
		env["REMOTE_PORT"] = port;

		TEnvMap::iterator iter;

		// 		iter = env.find("host");
		// 		if(env.end() != iter) env["SERVER_NAME"] = iter->second;

		iter = env.find("content-type");
		if(env.end() != iter)
		{
			std::string contentTypeLC = iter->second;
			tolowerLatin(contentTypeLC);
			std::string &contentType = iter->second;

			if(0 == contentTypeLC.compare(0, 33, "application/x-www-form-urlencoded")) connection->_inContentType = ectFormUrlEncoded;
			else if(0 == contentTypeLC.compare(0, 19, "multipart/form-data"))
			{
				size_t pos = contentTypeLC.find("boundary=");
				if(pos != std::string::npos)
				{
					connection->_inBodyBoundary.assign(contentType.begin() + pos + 9, contentType.end());
				}
				connection->_inContentType = ectFormData;
			}
			else
			{
				std::cerr<<"TransportAsio::processReadedHeader: unrecognized contentType "<<contentTypeLC<<std::endl;
				return;
			}

		}

		iter = env.find("content-length");
		if(env.end() != iter)
		{
			connection->_inContentLength = _atost(iter->second.data());
			if(connection->_inContentLength > _bodybufLimit)
			{
				std::cerr<<"TransportAsio::onCompleteProbe: body is too large"<<std::endl;
				return;
			}
			if(connection->_inContentType == ectNull)
			{
				connection->_inContentType = ectFictive;
			}
		}

		iter = env.find("connection");
		if(env.end() != iter)
		{
			tolowerLatin(iter->second);
			if(std::string::npos != iter->second.find("keep-alive"))
			{
				iter = env.find("keep-alive");
				if(env.end() != iter)
				{
					connection->_keepaliveTimeout = _atost(iter->second.data());
					if(connection->_keepaliveTimeout > _keepaliveMax) connection->_keepaliveTimeout = _keepaliveMax;
				}
				else
				{
					connection->_keepaliveTimeout = _keepaliveMax;
				}
			}
		}
		else
		{
			if(connection->_protocolVersion == "1.0")
			{
				connection->_keepaliveTimeout = 0;
			}
			else
			{
				connection->_keepaliveTimeout = _keepaliveMax;
			}
		}

		//вырезать заголовки из тела запроса
		connection->_dataBuf.erase(begin, end);
		connection->_bytesTransfered = connection->_dataBuf.size();
		connection->_netBuf.clear();

		//отдать бакенду на пробу
		connection->_state = ecsReadBody;

		if(_enableDeflate || _enableGzip)
		{
			TEnvMap::iterator iter = connection->_env.find("accept-encoding");
			if(connection->_env.end() != iter)
			{
				if(_enableGzip && std::string::npos != iter->second.find("gzip"))
				{
					connection->_outContentEncoding = eceGzip;
				}
				else if(_enableDeflate && std::string::npos != iter->second.find("deflate"))
				{
					connection->_outContentEncoding = eceDeflate;
				}
			}
		}

		if(!_staticDirectory.empty())
		{
			connection->_staticPath = _staticDirectory+connection->_requestPath;
			if(boost::filesystem::is_regular_file(connection->_staticPath))
			{
				onCompleteProbe_own(connection, true);
				return;
			}
			connection->_staticPath.clear();
		}


		pushProbe(connection);
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::processReadedBody(ConnectionPtr connection, std::size_t bytes_transferred)
	{
		connection->_bytesTransfered += bytes_transferred;

		switch(connection->_inContentType)
		{

		case ectFictiveNoProcess:
			{
				if(connection->_bytesTransfered >= connection->_inContentLength)
				{
					onCompleteProcess(connection, true);
					return;
				}
				size_t space2process = connection->_inContentLength - connection->_bytesTransfered;
				makeRead(TimerPtr(), connection, std::min((size_t)_bodybufGranula, space2process));
				return;
			}
			break;
		case ectNull:
		case ectFictive:
			{
				if(connection->_bytesTransfered >= connection->_inContentLength)
				{
					if(connection->_staticPath.empty())
					{
						pushProcess(connection);
					}
					else
					{
						processWriteStatic(connection);
					}
					return;
				}
				size_t space2process = connection->_inContentLength - connection->_bytesTransfered;
				makeRead(TimerPtr(), connection, std::min((size_t)_bodybufGranula, space2process));
				return;
			}
			break;
		case ectFormData:
		case ectFormUrlEncoded:
			if(bytes_transferred)
			{
				if(!readBodyStep(connection, bytes_transferred))
				{
					std::cerr<<"TransportAsio::processReadedBody: readBodyStep failed"<<std::endl;
					return;
				}
			}
			if(connection->_bytesTransfered >= connection->_inContentLength)
			{
				if(!readBodyStop(connection))
				{
					std::cerr<<"TransportAsio::processReadedBody: readBodyStop failed"<<std::endl;
					return;
				}
				if(connection->_staticPath.empty())
				{
					pushProcess(connection);
				}
				else
				{
					processWriteStatic(connection);
				}
				return;
			}
			else
			{
				size_t space2process = connection->_inContentLength - connection->_bytesTransfered;
				makeRead(TimerPtr(), connection, std::min((size_t)_bodybufGranula, space2process));
				return;
			}
			break;
		}
	}



	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::processWriteStatic(ConnectionPtr connection)
	{
		/////////////////////
		//время последнего изменения
		time_t mtime = -1;
		size_t size = 0;
		unsigned etag;

		if(_enableLastModified || _enableETag)
		{

#ifdef WIN32
			struct _stat st;
			int err = _stat(connection->_staticPath.data(), &st);
#else
			struct ::stat st;
			int err = ::stat(connection->_staticPath.data(), &st);
#endif
			if(err)
			{
				connection->_outStatus = 404;
				connection->_outBody = "404 Not Found";

				onCompleteProcess_own(connection, true);
				return;
			}

			mtime = st.st_mtime;
			size = st.st_size;
		}

		if(_enableLastModified)
		{
			TEnvMap::iterator iter = connection->_env.find("if-modified-since");
			if(connection->_env.end() != iter)
			{
				time_t mtimeBound = httpDate::parse(iter->second.data());
				if(mtimeBound >= mtime)
				{
					connection->_outStatus = 304;
					onCompleteProcess_own(connection, true);
					return;
				}
			}
		}

		///////////////////////////////
		//контрольный хэш
		if(_enableETag)
		{
			etag = Crc32(connection->_staticPath.data(), connection->_staticPath.size());
			etag += (unsigned)mtime;
			etag = Crc32((char *)&etag, sizeof(etag));
			etag += (unsigned)size;
			etag = Crc32((char *)&etag, sizeof(etag));

			TEnvMap::iterator iter = connection->_env.find("if-none-match");
			if(connection->_env.end() != iter)
			{
				unsigned alienTag = _atoui(trim_quotes(iter->second).data());
				if(alienTag == etag)
				{
					connection->_outStatus = 304;
					onCompleteProcess_own(connection, true);
					return;
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////
		connection->_staticFile.reset(new std::ifstream(connection->_staticPath.data(), std::ios::binary));

		if(!*connection->_staticFile)
		{
			connection->_outStatus = 404;
			connection->_outBody = "404 Not Found";

			onCompleteProcess_own(connection, true);
			return;
		}

		connection->_staticFileSize = size;


		size_t dotPos = connection->_staticPath.find_last_of('.');
		std::string mimeType;
		connection->_outCompression = -1;
		if(dotPos == std::string::npos)
		{
			mimeType = "application/octet-stream";
		}
		else
		{
			std::string ext = std::string(connection->_staticPath.begin()+dotPos+1, connection->_staticPath.end());
			mimeType = mimeTypeForExt(ext);
			connection->_outCompression = compressForExt(ext);
		}

		if(-1 > connection->_outCompression || 9 < connection->_outCompression || 0 == connection->_outCompression)
		{
			connection->_outContentEncoding = eceNone;
		}

		connection->_outStatus = 200;
		connection->_outHeaders += "Content-Type: " + mimeType+"\r\n";

		if(_enableLastModified)
		{
			connection->_outHeaders += "Last-Modified: " + httpDate::make(mtime) + "\r\n";
		}

		if(_enableETag)
		{
			connection->_outHeaders += "ETag: \"" + _ntoa(etag) + "\"\r\n";
		}

		////////////////////////////
		//deflate, gzip
		if(	connection->_outContentEncoding != eceNone && size)
		{
			if(size > 1024*1024)
			{
				if(connection->_protocolVersion == "1.0")
				{
					//в поточном варианте будет chunked, а он запрещен в HTTP/1.0
					connection->_outContentEncoding = eceNone;
				}
				else
				{
					//для больших - поточное сжатие, chunked
					if(eceGzip == connection->_outContentEncoding)
					{
						connection->_outCompressorStream.reset((CompressorStream*)new CompressorStreamGzip(connection->_outCompression));
					}
					else if(eceDeflate == connection->_outContentEncoding)
					{
						connection->_outCompressorStream.reset((CompressorStream*)new CompressorStreamDeflate(connection->_outCompression));
					}
					else
					{
						assert(!"unknown compressor");
					}
				}
			}
			else
			{
				//для маленьких - через буфер тела
				connection->_outBody.resize(size);
				connection->_staticFile->read((char *)connection->_outBody.data(), size);
				connection->_staticFile.reset();
			}
		}

		onCompleteProcess_own(connection, true);
	}


	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::processCompletedConnection(ConnectionPtr connection)
	{
		if(connection->_keepaliveTimeout)
		{
			ConnectionPtr newConnection(new Connection(this, connection->_socket, connection->_err));
			connection->_socket.reset();

			{
				std::size_t bytes_readable = newConnection->_socket->in_avail();
				if(bytes_readable)
				{
					makeRead(TimerPtr(), newConnection, _headerbufGranula);
					return;
				}
			}

			TimerPtr timer(new deadline_timer(_io_service));

			boost::system::error_code ec;
			timer->expires_from_now(
				boost::posix_time::seconds((long)connection->_keepaliveTimeout),
				ec); 
			if(ec)
			{
				std::cerr<<"TransportAsio::processCompletedConnection timer->expires_from_now failed"<<std::endl;
				//boost::system::error_code ec;
				//newConnection->_socket->shutdown(ip::tcp::socket::shutdown_both, ec);
				newConnection->_socket->close();
				return;
			}

			timer->async_wait(
				boost::bind(&TransportAsio::handleKeepaliveTimeout, this,
				timer,
				newConnection, 
				placeholders::error));

			makeRead(timer, newConnection, _headerbufGranula);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::onFullQueue(ConnectionPtr connection)
	{
		_io_service.post(
			boost::bind(
			&TransportAsio::onFullQueue_own, 
			this,
			connection));
	}


	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::onCompleteProbe(ConnectionPtr connection, bool res)
	{
		_io_service.post(
			boost::bind(
			&TransportAsio::onCompleteProbe_own, 
			this,
			connection,
			res));
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::onCompleteProcess(ConnectionPtr connection, bool res)
	{
		_io_service.post(
			boost::bind(
			&TransportAsio::onCompleteProcess_own, 
			this,
			connection,
			res));
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::onFullQueue_own(ConnectionPtr connection)
	{
		std::cerr<<"TransportAsio::onFullQueue"<<std::endl;
		//assert(0);
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::onCompleteProbe_own(ConnectionPtr connection, bool res)
	{
		if(res)
		{
			if(connection->_inContentLength)
			{
				if(!connection->_staticPath.empty())
				{
					if(ectNull != connection->_inContentType)
					{
						connection->_inContentType = ectFictive;
					}
					connection->_dataBuf.clear();
					connection->_netBuf.clear();
				}

				if(	connection->_inContentType == ectFormData ||
					connection->_inContentType == ectFormUrlEncoded)
				{
					if(!readBodyInit(connection))
					{
						std::cerr<<"TransportAsio::onCompleteProbe: readBodyInit failed"<<std::endl;
						return;
					}
				}
			}

			if(connection->_bytesTransfered >= connection->_inContentLength)
			{
				processReadedBody(connection, 0);
			}
			else
			{
				connection->_state = ecsReadBody;
				makeRead(TimerPtr(), connection, std::min((size_t)_bodybufGranula, connection->_inContentLength - connection->_bytesTransfered));
			}
			return;
		}
		else
		{
			connection->_inContentType = ectFictiveNoProcess;
			connection->_dataBuf.clear();
			connection->_netBuf.clear();
			if(connection->_bytesTransfered >= connection->_inContentLength)
			{
				processReadedBody(connection, 0);
			}
			else
			{
				connection->_state = ecsReadBody;
				makeRead(TimerPtr(), connection, std::min((size_t)_bodybufGranula, connection->_inContentLength - connection->_bytesTransfered));
			}
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::onCompleteProcess_own(ConnectionPtr connection, bool res)
	{
		if(res)
		{
			if(connection->_staticPath.empty() || connection->_staticFile || connection->_outStatus)
			{
				if(!connection->_outStatus) connection->_outStatus = 500;
				prepareHeaders(connection);
				connection->_state = ecsWriteHeader;
				connection->_bytesTransfered = 0;
				makeWrite(connection);
			}
			else
			{
				processWriteStatic(connection);
			}

			return;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::onConectionDestroy(Connection *connection)
	{
		if(connection->_backendData)
		{
			pushCleanup(connection->_backendData);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::prepareHeaders(ConnectionPtr connection)
	{
		std::string first = "HTTP/" + connection->_protocolVersion + " ";

		const char *statusText;
		switch(connection->_outStatus)
		{
		case 100: statusText = "100 Continue\r\n"; break;
		case 101: statusText = "101 Switching Protocols\r\n"; break;
		case 200: statusText = "200 OK\r\n"; break;
		case 201: statusText = "201 Created\r\n"; break;
		case 202: statusText = "202 Accepted\r\n"; break;
		case 203: statusText = "203 Non-Authoritative Information\r\n"; break;
		case 204: statusText = "204 No Content\r\n"; break;
		case 206: statusText = "206 Partial Content\r\n"; break;
		case 300: statusText = "300 Multiple Choices\r\n"; break;
		case 301: statusText = "301 Moved Permanently\r\n"; break;
		case 302: statusText = "302 Found\r\n"; break;
		case 303: statusText = "303 See Other\r\n"; break;
		case 304: statusText = "304 Not Modified\r\n"; break;
		case 305: statusText = "305 Use Proxy\r\n"; break;
		case 307: statusText = "307 Temporary Redirect\r\n"; break;
		case 400: statusText = "400 Bad Request\r\n"; break;
		case 401: statusText = "401 Unauthorized\r\n"; break;
		case 402: statusText = "402 Payment Required\r\n"; break;
		case 403: statusText = "403 Forbidden\r\n"; break;
		case 404: statusText = "404 Not Found\r\n"; break;
		case 405: statusText = "405 Method Not Allowed\r\n"; break;
		case 406: statusText = "406 Not Acceptable\r\n"; break;
		case 407: statusText = "407 Proxy Authentication Required\r\n"; break;
		case 408: statusText = "408 Request Timeout\r\n"; break;
		case 409: statusText = "409 Conflict\r\n"; break;
		case 410: statusText = "410 Gone\r\n"; break;
		case 411: statusText = "411 Length Required\r\n"; break;
		case 412: statusText = "412 Precondition Failed\r\n"; break;
		case 413: statusText = "413 Request Entity Too Large\r\n"; break;
		case 414: statusText = "414 Request-URI Too Long\r\n"; break;
		case 415: statusText = "415 Unsupported Media Type\r\n"; break;
		case 416: statusText = "416 Requested Range Not Satisfiable\r\n"; break;
		case 417: statusText = "417 Expectation Failed\r\n"; break;
		default:
		case 500: statusText = "500 Internal Server Error\r\n"; break;
		case 501: statusText = "501 Not Implemented\r\n"; break;
		case 502: statusText = "502 Bad Gateway\r\n"; break;
		case 503: statusText = "503 Service Unavailable\r\n"; break;
		case 504: statusText = "504 Gateway Timeout\r\n"; break;
		case 505: statusText = "505 HTTP Version Not Supported\r\n"; break;
		}

		first += statusText;
		connection->_outHeaders.insert(0, first);

		if(!connection->_outBody.empty())
		{

			if(connection->_outContentEncoding != eceNone)
			{
				bool encoded = false;
				////////////////////////////
				//deflate
				if(connection->_outContentEncoding == eceDeflate)
				{
					if(stringDeflate(connection->_outBody, connection->_outCompression))
					{
						connection->_outHeaders += "Content-Encoding: deflate\r\n";
						encoded = true;
					}
				}
				////////////////////////////
				//gzip
				if(!encoded && connection->_outContentEncoding == eceGzip)
				{
					if(stringGzip(connection->_outBody, connection->_outCompression))
					{
						connection->_outHeaders += "Content-Encoding: gzip\r\n";
						encoded = true;
					}
				}

			}

			connection->_outHeaders += "Content-Length: " + _ntoa(connection->_outBody.size())+ "\r\n";
		}
		else if(connection->_outCompressorStream)
		{
			if(connection->_outContentEncoding == eceDeflate)
					connection->_outHeaders += "Content-Encoding: deflate\r\n";
			else if(connection->_outContentEncoding == eceGzip)
					connection->_outHeaders += "Content-Encoding: gzip\r\n";
			else
				assert(!"unknown compressor!!!");
			connection->_outHeaders += "Transfer-Encoding: chunked\r\n";
		}
		else if(connection->_staticFile)
		{
			connection->_outHeaders += "Content-Length: " + _ntoa(connection->_staticFileSize)+ "\r\n";
		}
		else
		{
			connection->_outHeaders += "Content-Length: 0\r\n";
		}


		////////////////////////////
		if(connection->_keepaliveTimeout)
		{
			connection->_outHeaders += "Connection: Keep-Alive\r\n";
		}
		else
		{
			connection->_outHeaders += "Connection: Close\r\n";
		}

		////////////////////////////
		connection->_outHeaders += "\r\n";
	}

	//////////////////////////////////////////////////////////////////////////
	std::string TransportAsio::mimeTypeForExt(const std::string &ext)
	{
		std::map<std::string, std::string>::iterator iter = _extMimeTypes.find(ext);
		if(_extMimeTypes.end() == iter)
		{
			return "application/octet-stream";
		}

		return iter->second;
	}

	//////////////////////////////////////////////////////////////////////////
	int TransportAsio::compressForExt(const std::string &ext)
	{
		std::map<std::string, int>::iterator iter = _extCompress.find(ext);
		if(_extCompress.end() == iter)
		{
			return -1;
		}

		return iter->second;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TransportAsio::parsePramsGet(ConnectionPtr connection, std::string::iterator begin, std::string::iterator end)
	{
		tokenizer<std::string> tok(begin, end, "&");
		tokenizer<std::string>::iterator tok_iter = tok.begin();
		tokenizer<std::string>::iterator tok_end = tok.end();

		for(; tok_iter!=tok_end; tok_iter++)
		{
			std::string::iterator eqIter = std::find(tok_iter->_begin, tok_iter->_end, '=');
			if(tok_iter->_end == eqIter)
			{//key
				std::string key(tok_iter->_begin, tok_iter->_end);
				dropInvalidUtf8(urldecode(key));
				connection->_paramsGet[key].push_back(std::string());
			}
			else
			{//key=value
				std::string key(tok_iter->_begin, eqIter);
				dropInvalidUtf8(urldecode(key));
				std::string value(eqIter+1, tok_iter->_end);
				dropInvalidUtf8(urldecode(value));
				connection->_paramsGet[key].push_back(value);
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TransportAsio::parseCookies(ConnectionPtr connection, std::string::iterator begin, std::string::iterator end)
	{
		tokenizer<std::string> tok(begin, end, "; ");
		tokenizer<std::string>::iterator tok_iter = tok.begin();
		tokenizer<std::string>::iterator tok_end = tok.end();

		for(; tok_iter!=tok_end; tok_iter++)
		{
			std::string::iterator eqIter = std::find(tok_iter->_begin, tok_iter->_end, '=');
			if(tok_iter->_end == eqIter)
			{//key
				std::string key(tok_iter->_begin, tok_iter->_end);
				dropInvalidUtf8(urldecode(key));
				connection->_cookies[key].push_back(std::string());
			}
			else
			{//key=value
				std::string key(tok_iter->_begin, eqIter);
				dropInvalidUtf8(urldecode(key));
				std::string value(eqIter+1, tok_iter->_end);
				dropInvalidUtf8(urldecode(value));
				connection->_cookies[key].push_back(value);
			}
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool TransportAsio::readBodyInit(ConnectionPtr connection)
	{
		switch(connection->_inContentType)
		{
		case ectFormData:
			{
				static const char MM[] = "--";
				connection->_inBodyBoundary.insert(connection->_inBodyBoundary.begin(), MM, MM+2);
				connection->_fdState = efdsInit;
				connection->_fdBuffer.clear();
				readParamsStep_fdReset(connection);
			}
			break;
		case ectFormUrlEncoded:
			connection->_fueWaitKey = true;
			connection->_fueAccumulerKey.clear();
			connection->_fueAccumulerValue.clear();
			break;
		default:
			assert(0);
			return false;
		}

		if(!connection->_dataBuf.empty())
		{
			if(!readParamsStep(connection, connection->_dataBuf.begin(), connection->_dataBuf.end()))
			{
				return false;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TransportAsio::readBodyStep(ConnectionPtr connection, size_t butesTransfered)
	{
		assert(butesTransfered);
		if(!readParamsStep(connection, connection->_netBuf.begin(), connection->_netBuf.begin()+butesTransfered))
		{
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TransportAsio::readBodyStop(ConnectionPtr connection)
	{
		switch(connection->_inContentType)
		{
		case ectFormData:
			readParamsStep_fdReset(connection);
			if(connection->_fdState != efdsComplete)
			{
				std::cerr<<"TransportAsio::readBodyStop uncomplete form data"<<std::endl;
				return false;
			}
			break;
		case ectFormUrlEncoded:
			if(!connection->_fueAccumulerKey.empty() && !connection->_fueAccumulerValue.empty())
			{
				connection->_paramsPost[dropInvalidUtf8(urldecode(connection->_fueAccumulerKey))].push_back(dropInvalidUtf8(urldecode(connection->_fueAccumulerValue)));
			}

			connection->_fueWaitKey = true;
			connection->_fueAccumulerKey.clear();
			connection->_fueAccumulerValue.clear();
			break;
		default:
			assert(0);
			return false;
		}
		connection->_netBuf.clear();


		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Iterator>
	bool TransportAsio::readParamsStep(ConnectionPtr connection, Iterator begin, Iterator end)
	{
		switch(connection->_inContentType)
		{
		case ectFormData:
			connection->_fdBuffer.insert(connection->_fdBuffer.end(), begin, end);

			for(;;)
			{
				switch(connection->_fdState)
				{
				case efdsInit:
					//boundary [размер]
					if(connection->_fdBuffer.size() < connection->_inBodyBoundary.size())
					{
						//продолжать читать
						return true;
					}
					if(!std::equal(connection->_inBodyBoundary.begin(), connection->_inBodyBoundary.end(), connection->_fdBuffer.begin()))
					{
						//первая граница не найдена
						std::cerr<<"TransportAsio::readParamsStep: fd initial boundary out of format"<<std::endl;
						return false;
					}

					//вытереть первую границу
					connection->_fdBuffer.erase(connection->_fdBuffer.begin(), connection->_fdBuffer.begin()+connection->_inBodyBoundary.size());
					connection->_fdState = efdsProcessHeader;

					//break;
				case efdsProcessHeader:

					while(efdsProcessHeader == connection->_fdState)
					{
						//--\r\n конец, иначе в данные
						if(	connection->_fdBuffer.size() >= 4 &&
							connection->_fdBuffer[0] == '-' &&
							connection->_fdBuffer[1] == '-' &&
							connection->_fdBuffer[2] == '\r' &&
							connection->_fdBuffer[3] == '\n')
						{
							connection->_fdBuffer.erase(connection->_fdBuffer.begin(), connection->_fdBuffer.begin()+4);
							connection->_fdState = efdsComplete;
							break;
						}

						//\r\n Key: Value [\r\n]
						if(connection->_fdBuffer.size() < 4) //"\r\n\r\n"
						{
							//продолжать читать
							return true;
						}
						if(	connection->_fdBuffer[0] != '\r' || 
							connection->_fdBuffer[1] != '\n' )
						{
							//первым должен перевод строки
							std::cerr<<"TransportAsio::readParamsStep: fd header out of format"<<std::endl;
							return false;
						}

						//потом должно <ключ: значение> и перевод строки
						static const char CRLF[] = "\r\n";
						DataBuf::iterator iterLineEnd = std::search(connection->_fdBuffer.begin()+2, connection->_fdBuffer.end(), CRLF, CRLF+2);
						if(connection->_fdBuffer.end() == iterLineEnd)
						{
							//продолжать читать
							return true;
						}

						//заголовок найден
						if(iterLineEnd == connection->_fdBuffer.begin() + 2)
						{
							//пустая линия - конец заголовка
							connection->_fdState = efdsProcessBody;

							//вытереть отработанную строку и перевод. Теперь в буфере начало данных
							connection->_fdBuffer.erase(connection->_fdBuffer.begin(), iterLineEnd+2);
							break;
						}
						else
						{
							//линия не пустая, взять ключ значение
							static const char D2SP[] = ": ";
							DataBuf::iterator iterD2sp = std::search(connection->_fdBuffer.begin()+2, iterLineEnd-2, D2SP, D2SP+2);
							if(connection->_fdBuffer.end() == iterD2sp)
							{
								std::cerr<<"TransportAsio::readParamsStep: fd header line out of format"<<std::endl;
								return false;
							}
							// 							std::string key(connection->_fdBuffer.begin()+2, iterD2sp);
							// 							std::string value(iterD2sp+2, iterLineEnd);
							// 							std::cout<<"Header data: "<<key<<"="<<value<<std::endl;

							if(!readParamsStep_fdHeader(connection, 
								connection->_fdBuffer.begin()+2, iterD2sp,
								iterD2sp+2, iterLineEnd))
							{
								return false;
							}

							//вытереть отработанную строку, последний перевод строки оставить
							connection->_fdBuffer.erase(connection->_fdBuffer.begin(), iterLineEnd);
						}
					}
					break;
				case efdsProcessBody:
					// data [\r\nboundary]
					{
						if(connection->_fdBuffer.size() < connection->_inBodyBoundary.size()+2)
						{
							//мало данных, продолжать читать
							return true;
						}

						DataBuf::iterator iterBoundary = std::search(
							connection->_fdBuffer.begin(), 
							connection->_fdBuffer.end(), 
							connection->_inBodyBoundary.begin(),
							connection->_inBodyBoundary.end());

						if(	iterBoundary == connection->_fdBuffer.end() ||
							iterBoundary - connection->_fdBuffer.begin() < 2 &&
							'\r' != *(iterBoundary-2) &&
							'\n' != *(iterBoundary-1))
						{
							//не она, выбрать доступные данные и читать дальше
							DataBuf::iterator dataEnd = connection->_fdBuffer.end()-(1+connection->_inBodyBoundary.size());
							// 							std::string data(connection->_fdBuffer.begin(), dataEnd);
							// 							std::cout<<"fdd part["<<data<<"]"<<std::endl;
							if(!readParamsStep_fdData(connection, connection->_fdBuffer.begin(), dataEnd, false))
							{
								return false;
							}
							connection->_fdBuffer.erase(connection->_fdBuffer.begin(), dataEnd);
							return true;
						}

						//найдена граница, выбрать доступные данные и уйти на следующую итерацию
						// 						std::string data(connection->_fdBuffer.begin(), iterBoundary-2);
						// 						std::cout<<"fdd last["<<data<<"]"<<std::endl;
						if(!readParamsStep_fdData(connection, connection->_fdBuffer.begin(), iterBoundary-2, true))
						{
							return false;
						}
						connection->_fdBuffer.erase(connection->_fdBuffer.begin(), iterBoundary+connection->_inBodyBoundary.size());
						connection->_fdState = efdsProcessHeader;
					}

					break;
				case efdsComplete:
					if(	connection->_bytesTransfered < connection->_inContentLength ||
						!connection->_fdBuffer.empty())
					{
						std::cerr<<"TransportAsio::readParamsStep: fd extra data after mime"<<std::endl;
						return false;
					}
					return true;
				}
			}
			break;
		case ectFormUrlEncoded:
			{
				while(begin != end)
				{
					if(connection->_fueWaitKey)
					{//key
						Iterator iter = std::find(begin, end, '=');
						if(end == iter)
						{
							connection->_fueAccumulerKey.insert(connection->_fueAccumulerKey.end(), begin, end);
							begin = end;
						}
						else
						{
							connection->_fueAccumulerKey.insert(connection->_fueAccumulerKey.end(), begin, iter);
							iter++;
							begin = iter;
							connection->_fueWaitKey = false;
						}
					}

					if(!connection->_fueWaitKey)
					{//value
						Iterator iter = std::find(begin, end, '&');
						if(end == iter)
						{
							connection->_fueAccumulerValue.insert(connection->_fueAccumulerValue.end(), begin, end);
							begin = end;
						}
						else
						{
							connection->_fueAccumulerValue.insert(connection->_fueAccumulerValue.end(), begin, iter);
							if(!connection->_fueAccumulerKey.empty() && !connection->_fueAccumulerValue.empty())
							{
								connection->_paramsPost[dropInvalidUtf8(urldecode(connection->_fueAccumulerKey))].push_back(dropInvalidUtf8(urldecode(connection->_fueAccumulerValue)));
							}
							connection->_fueAccumulerKey.clear();
							connection->_fueAccumulerValue.clear();
							iter++;
							begin = iter;
							connection->_fueWaitKey = true;
						}
					}
				}
			}
			break;
		default:
			assert(0);
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void TransportAsio::readParamsStep_fdReset(ConnectionPtr connection)
	{
		connection->_fdName.clear();
		connection->_fdFileName.clear();
		connection->_fdContentType.clear();
		connection->_fdFile.reset();
		connection->_fdValue.clear();
	}


	//////////////////////////////////////////////////////////////////////////
	bool TransportAsio::readParamsStep_fdHeader(ConnectionPtr connection, 
		DataBuf::iterator keyBegin, 
		DataBuf::iterator keyEnd,
		DataBuf::iterator valueBegin,
		DataBuf::iterator valueEnd)
	{
		tolowerLatin(keyBegin, keyEnd);

		if(	keyEnd - keyBegin == 19 && 
			std::equal(keyBegin, keyEnd, "content-disposition"))
		{
			//std::cerr<<"content-disposition"<<std::endl;
			tokenizer<DataBuf> tok(valueBegin, valueEnd, "; ");
			tokenizer<DataBuf>::iterator tok_iter = tok.begin();
			tokenizer<DataBuf>::iterator tok_end = tok.end();

			for(; tok_iter!=tok_end; tok_iter++)
			{
				const IteratorPair<DataBuf::iterator> &kv = *tok_iter;
				DataBuf::iterator i1 = std::find(kv._begin, kv._end, '=');
				if(i1 == kv._end)
				{
					// 					std::string data(kv._begin, kv._end);
					// 					std::cerr<<"main: ["<<data<<"]"<<std::endl;
					tolowerLatin(kv._begin, kv._end);
					if(kv._end - kv._begin != 9 ||
						!std::equal(kv._begin, kv._end, "form-data"))
					{
						std::cerr<<"TransportAsio::readParamsStep_fdHeader: content-disposition is not recognized"<<std::endl;
						return false;
					}
				}
				else
				{
					// 					std::string key(kv._begin, i1);
					// 					std::string val(i1+1, kv._end);
					// 					std::cerr<<"kv: ["<<key<<"] = ["<<val<<"]"<<std::endl;
					tolowerLatin(kv._begin, i1);
					if(i1 - kv._begin == 4 &&
						std::equal(kv._begin, i1, "name"))
					{
						connection->_fdName.assign(i1+1, kv._end);
						trim_quotes(connection->_fdName);
					}
					else if(i1 - kv._begin == 8 &&
						std::equal(kv._begin, i1, "filename"))
					{
						connection->_fdFileName.assign(i1+1, kv._end);
						trim_quotes(connection->_fdFileName);
					}
				}
			}
		}
		else if(	keyEnd - keyBegin == 12 && 
			std::equal(keyBegin, keyEnd, "content-type"))
		{
			// 			std::string data(valueBegin, valueEnd);
			// 			std::cerr<<"content-type: ["<<data<<"]"<<std::endl;
			connection->_fdContentType.assign(valueBegin, valueEnd);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TransportAsio::readParamsStep_fdData(ConnectionPtr connection, 
		DataBuf::iterator dataBegin, 
		DataBuf::iterator dataEnd,
		bool isLast)
	{
		if(connection->_fdFileName.empty())
		{//простой
			connection->_fdValue.insert(connection->_fdValue.end(), dataBegin, dataEnd);

			if(isLast)
			{
				connection->_paramsPost[connection->_fdName].push_back(connection->_fdValue);
			}
		}
		else
		{//файл
			if(!connection->_fdFile)
			{
#ifdef WIN32
				char dir[MAX_PATH-14];
				char fil[MAX_PATH];
				if(	!GetTempPathA(sizeof(dir), dir) ||
					!GetTempFileNameA(dir, "ccms_ta_", 0, fil))
					//if(tmpnam_s(buf, sizeof(buf)))
				{
					std::cerr<<"TransportAsio::readParamsStep_fdData: unable to obtain temporary file name"<<std::endl;
					return false;
				}
#else
				char fil[256] = "/tmp/ccms_ta_XXXXXX";
				if(!mktemp(fil))
				{
					std::cerr<<"TransportAsio::readParamsStep_fdData unable to obtain temporary file name: "<<strerror(errno)<<"("<<errno<<")"<<std::endl;
					return false;
				}
#endif
				connection->_fdValue = fil;
				connection->_fdFile.reset(new std::ofstream(fil, std::ios::binary));
				if(!*connection->_fdFile)
				{
					std::cerr<<"TransportAsio::readParamsStep_fdData: unable to open temporary file"<<std::endl;
					return false;
				}
			}
			else
			{
				if(!*connection->_fdFile)
				{
					std::cerr<<"TransportAsio::readParamsStep_fdData: temporary file is closed"<<std::endl;
					return false;
				}
			}

			size_t dataLen = dataEnd - dataBegin;
			if(dataLen)
			{
				if(!connection->_fdFile->write(&*dataBegin, dataLen))
				{
					std::cerr<<"TransportAsio::readParamsStep_fdData: unable to write in temporary file"<<std::endl;
					return false;
				}
			}

			if(isLast)
			{
				ParamFile pf;
				pf._nameClient = connection->_fdFileName;
				pf._nameServer = connection->_fdValue;
				pf._contentType = connection->_fdContentType;
				pf._size = connection->_fdFile->tellp();

				connection->_fdFile.reset();
				connection->_files[connection->_fdName].push_back(pf);
			}
		}


		if(isLast)
		{
			readParamsStep_fdReset(connection);
		}
		return true;
	}
}
