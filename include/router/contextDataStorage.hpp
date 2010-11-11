#ifndef _ccms_contextDataStorage_hpp_
#define _ccms_contextDataStorage_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
{
	class ContextData;

	//////////////////////////////////////////////////////////////////////////
	class ContextDataStorage
	{
	public:
		typedef std::map<std::string, jsval> TMInstances;

	public:
		ContextDataStorage();
		~ContextDataStorage();

		//внешний интерфейс. Начать сессию / окончить сессию
		bool startInstance(const std::string &key);
		bool stopInstance(const std::string &key);
		void reset();

		//полный экземпляр данных
		jsval getInstance(const std::string &key);
		bool setInstance(const std::string &key, jsval v);
		const TMInstances getInstances();

		void registerCd(ContextData *cd);
		void unregisterCd(ContextData *cd);
		void unregisterCd_gcPossible(ContextData *cd);

		void flushUnregisterCarriers();

	public:
		//внутренний интерфейс
		//доступ к части данных для носителя
		JSObject *getData4Carrier(const std::string &key, jsid carrier);
		void resetData4Carrier(const std::string &key, jsid carrier);

	private:
		TMInstances _instances;

		typedef std::set<ContextData *> TSCds;
		TSCds _cds;

		typedef std::set<std::string> TSCarriers;
		TSCarriers _unregisteredCarriers;
	};
}
#endif
