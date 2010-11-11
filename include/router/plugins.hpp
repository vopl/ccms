#ifndef _ccms_plugins_hpp_
#define _ccms_plugins_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
{
	class Plugins
		: public JsObject
	{
	public:
		Plugins();
		~Plugins();

	public:
		bool call_reload(uintN argc, jsval *argv, jsval *rval);
		bool call_serialize(uintN argc, jsval *argv, jsval *rval);
		bool call_deserialize(uintN argc, jsval *argv, jsval *rval);

	private:
		void reload(const std::vector<std::string> &dirs);

	};
	typedef JsPtr<Plugins> PluginsPtr;
}
#endif
