#ifndef _ccms_mime_headerAddress_hpp_
#define _ccms_mime_headerAddress_hpp_

#include "mime/headerBase.hpp"

namespace ccms{namespace mime{

	class HeaderAddress
		: public HeaderBase
	{
	public:
		HeaderAddress();
		~HeaderAddress();

		bool construct(uintN argc, jsval *argv);
		bool jsRegister();
	};
	typedef JsPtr<HeaderAddress> HeaderAddressPtr;


	//////////////////////////////////////////////////////////////////////////
	class HeaderAddressConstructor
		: public JsObject
	{
	public:
		HeaderAddressConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}
#endif
