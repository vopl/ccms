#ifndef _ccms_transportBase_h_
#define _ccms_transportBase_h_

#include "iTransport.hpp"
#include <queue>

namespace ccms
{
	template<class Connection4TransportPtr>
	class TransportBase
		: public ITransport
	{
	protected:
		std::string		_host;
		unsigned short	_port;
		unsigned short	_portSsl;
		size_t			_queueSize;
		volatile bool	_stop;

		TEnvMap				_commonEnv;
		ITransportBackend	*_backend;

	public:
		TransportBase(const char *host, unsigned short port, unsigned short	portSsl, size_t	queueSize, ITransportBackend *backend);
		virtual ~TransportBase();
		virtual bool start();
		virtual bool stop();

	protected:
		void pushProbe(Connection4TransportPtr connection);
		void pushProcess(Connection4TransportPtr connection);
		void pushCleanup(void *connectionData);
		void pushCronTick();

		virtual void onFullQueue(Connection4TransportPtr connection) = 0;
		virtual void onCompleteProbe(Connection4TransportPtr connection, bool res) = 0;
		virtual void onCompleteProcess(Connection4TransportPtr connection, bool res) = 0;
	private:
		boost::thread _backendThread;
		void backendThreadProc(ITransportBackend *backend);

	private:
		typedef std::deque<Connection4TransportPtr> TDConnections4Transport;
		typedef std::deque<void *> TDConnectionDatas;

		boost::condition_variable_any	_cv;
		boost::recursive_mutex			_cvMtx;//рекурсивный захват возможен из потока бакенда, по окончании process если запрос будет исполнен транспортом быстрее чем поток бакенда избавится от ссылки на коннект
		volatile bool					_cvEvent;

		boost::recursive_mutex		_connectionsMtx_;
		TDConnections4Transport		_connections2probe;
		TDConnections4Transport		_connections2process;
		TDConnectionDatas			_connectionDatas2cleanup;
		bool						_cronTick;
	};

}
#endif
