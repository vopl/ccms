#ifndef _ccms_mime_headerText_hpp_
#define _ccms_mime_headerText_hpp_

#include "mime/headerBase.hpp"

namespace ccms{namespace mime{

	class HeaderText
		: public HeaderBase
	{
	public:
		HeaderText();
		~HeaderText();

		bool construct(uintN argc, jsval *argv);
		bool jsRegister();
	};
	typedef JsPtr<HeaderText> HeaderTextPtr;


	//////////////////////////////////////////////////////////////////////////
	class HeaderTextConstructor
		: public JsObject
	{
	public:
		HeaderTextConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}
#endif
