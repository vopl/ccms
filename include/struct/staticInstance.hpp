#ifndef _ccms_staticInstance_hpp_
#define _ccms_staticInstance_hpp_

#include "scripter/jsObject.hpp"
#include "scripter/scripter.hpp"
#include "utils/initializable.hpp"
#include "assocSeqContainer.hpp"

namespace ccms
{

	class PointInstance;

	//////////////////////////////////////////////////////////////////////////
	class StaticInstance
		: public JsObject
		, public Initializable
	{
		friend class Router;
		std::string				_fileName;
		PointInstance			*_parent;
	public:
		StaticInstance();
		~StaticInstance();

		virtual void init();
		virtual void deinit();
		void setParent(PointInstance *parent);

		std::string &getFileName();

	private:
	};

	typedef JsPtr<StaticInstance> StaticInstancePtr;
	typedef AssocSeqContainer<std::string, StaticInstancePtr> TMStaticInstances;
}
#endif
