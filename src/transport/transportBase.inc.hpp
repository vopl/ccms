#ifndef _ccms_transportBase_inc_hpp_
#define _ccms_transportBase_inc_hpp_

#include "boost/date_time/posix_time/posix_time.hpp"

namespace ccms
{

	//////////////////////////////////////////////////////////////////////////
	template<class Connection4TransportPtr>
	TransportBase<Connection4TransportPtr>::TransportBase(
		const char *host, 
		unsigned short port, 
		size_t queueSize, 
		ITransportBackend *backend)
		: _host(host?host:"localhost")
		, _port(port?port:80)
		, _queueSize(queueSize)
		, _backend(backend)
		, _stop(false)
		, _cvEvent(false)
		, _cronTick(false)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	template<class Connection4TransportPtr>
	TransportBase<Connection4TransportPtr>::~TransportBase()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	template<class Connection4TransportPtr>
	bool TransportBase<Connection4TransportPtr>::start()
	{
		if(!_commonEnv.empty()) return false;

		_stop = false;
		_cronTick = false;
		_cvEvent = false;
		_backendThread = boost::thread(
			boost::bind(&TransportBase::backendThreadProc, this, _backend)
			);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template<class Connection4TransportPtr>
	bool TransportBase<Connection4TransportPtr>::stop()
	{
		_stop = true;
		do 
		{
			{
				boost::lock_guard<boost::recursive_mutex> sl(_cvMtx);
				_cvEvent = true;
			}
			_cv.notify_all();
		}
		while(!_backendThread.timed_join(boost::posix_time::milliseconds(50)));

		_commonEnv.clear();

		_connections2probe.clear();
		_connections2process.clear();
		_connectionDatas2cleanup.clear();

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template<class Connection4TransportPtr>
	void TransportBase<Connection4TransportPtr>::pushProbe(Connection4TransportPtr connection)
	{
		if(_stop) return;

		{
			boost::lock_guard<boost::recursive_mutex> sl(_connectionsMtx_);
			_connections2probe.push_back(connection);
		}

		{
			boost::lock_guard<boost::recursive_mutex> sl(_cvMtx);
			_cvEvent = true;
		}
		_cv.notify_one();

	}

	//////////////////////////////////////////////////////////////////////////
	template<class Connection4TransportPtr>
	void TransportBase<Connection4TransportPtr>::pushCleanup(void *connectionData)
	{
		if(_stop) return;

		{
			boost::lock_guard<boost::recursive_mutex> sl(_connectionsMtx_);
			_connectionDatas2cleanup.push_back(connectionData);
		}

		{
			boost::lock_guard<boost::recursive_mutex> sl(_cvMtx);
			_cvEvent = true;
		}
		_cv.notify_one();

	}

	//////////////////////////////////////////////////////////////////////////
	template<class Connection4TransportPtr>
	void TransportBase<Connection4TransportPtr>::pushProcess(Connection4TransportPtr connection)
	{
		if(_stop) return;

		{
			boost::lock_guard<boost::recursive_mutex> sl(_connectionsMtx_);
			_connections2process.push_back(connection);
		}

		{
			boost::lock_guard<boost::recursive_mutex> sl(_cvMtx);
			_cvEvent = true;
		}
		_cv.notify_one();
	}

	//////////////////////////////////////////////////////////////////////////
	template<class Connection4TransportPtr>
	void TransportBase<Connection4TransportPtr>::pushCronTick()
	{
		if(_stop) return;

		{
			boost::lock_guard<boost::recursive_mutex> sl(_connectionsMtx_);
			_cronTick = true;
		}

		{
			boost::lock_guard<boost::recursive_mutex> sl(_cvMtx);
			_cvEvent = true;
		}
		_cv.notify_one();

	}


	//////////////////////////////////////////////////////////////////////////
	template<class Connection4TransportPtr>
	void TransportBase<Connection4TransportPtr>::backendThreadProc(ITransportBackend *backend)
	{
		try
		{
			if(!backend->initializeThread())
			{
				std::cerr<<"backend initialization failed";
				return;
			}

			for(;;)
			{

				//ожидание
				{
					boost::unique_lock<boost::recursive_mutex> lock(_cvMtx);

					while(!_cvEvent)
					{
						_cv.wait(_cvMtx);
					}
					_cvEvent = false;
				}

				//исполнить останов
				if(_stop)
				{
					break;
				}


				for(;;)
				{
					//забрать входящие данные
					TDConnections4Transport		connections2probe;
					TDConnections4Transport		connections2process;
					TDConnectionDatas			connectionDatas2cleanup;
					bool						cronTick = false;

					{
						boost::unique_lock<boost::recursive_mutex> lock(_connectionsMtx_);
						connections2probe.swap(_connections2probe);
						connections2process.swap(_connections2process);
						connectionDatas2cleanup.swap(_connectionDatas2cleanup);
						std::swap(cronTick, _cronTick);
					}

					if(	connections2probe.empty() &&
						connections2process.empty() &&
						connectionDatas2cleanup.empty() &&
						!cronTick)
					{
						break;
					}


					//исполнять
					BOOST_FOREACH(Connection4TransportPtr &c, connections2probe)
					{
						onCompleteProbe(c, backend->probe(c.get()));
						c.reset();
					}

					BOOST_FOREACH(Connection4TransportPtr &c, connections2process)
					{
						onCompleteProcess(c, backend->process(c.get()));
						c.reset();
					}

					BOOST_FOREACH(void *cd, connectionDatas2cleanup)
					{
						backend->cleanup(cd);
					}

					if(cronTick)
					{
						backend->cronTick(std::cerr);
					}
				}
			}

			backend->deinitializeThread();
		}
		catch(...)
		{
			std::cerr<<"exception in backend thread. WTF?"<<std::endl;
		}
	}

}
#endif
