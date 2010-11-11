#ifndef _ccms_static_hpp_
#define _ccms_static_hpp_

#include "scripter/jsObject.hpp"
#include "struct/staticInstance.hpp"

namespace ccms
{

	class Static
		: public JsObject
		, public Initializable
	{
	public:
		Static(JsObject *parent, StaticInstancePtr instance);
		~Static();

		virtual void init();
		virtual void deinit();

		const StaticInstancePtr &getInstance();

		const std::string &getFileName();

	private:
		JsObject *_parent;
		StaticInstancePtr _instance;
	};

	typedef JsPtr<Static> StaticPtr;
	typedef AssocSeqContainer<std::string, StaticPtr> TMStatics;
}
#endif
