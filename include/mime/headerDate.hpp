#ifndef _ccms_mime_headerDate_hpp_
#define _ccms_mime_headerDate_hpp_

#include "mime/headerBase.hpp"

namespace ccms{namespace mime{

	class HeaderDate
		: public HeaderBase
	{
	public:
		HeaderDate();
		~HeaderDate();

		bool construct(uintN argc, jsval *argv);
		bool jsRegister();
	};
	typedef JsPtr<HeaderDate> HeaderDatePtr;


	//////////////////////////////////////////////////////////////////////////
	class HeaderDateConstructor
		: public JsObject
	{
	public:
		HeaderDateConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}
#endif
