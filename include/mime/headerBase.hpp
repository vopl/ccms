#ifndef _ccms_mime_headerBase_hpp_
#define _ccms_mime_headerBase_hpp_

#include "scripter/jsObject.hpp"

namespace ccms{namespace mime{

	class HeaderBase
		: public JsObject
	{
		std::string _name;
	public:
		HeaderBase(const char*jsName);
		~HeaderBase();

		const char *getName() const;
		void setName(const char *name);
	};
}}
#endif
