#ifndef _ccms_mime_headers_hpp_
#define _ccms_mime_headers_hpp_

#include "scripter/jsObject.hpp"
#include "mime/headerBase.hpp"

namespace ccms{namespace mime{

	class Headers
		: public JsObject
	{
	public:
		Headers();
		~Headers();

		bool construct(uintN argc, jsval *argv);
		bool jsRegister();

		bool call_value(uintN argc, jsval *argv, jsval *rval);

	private:
		bool pushHeader(HeaderBase *value);
		HeaderBase *parseHeader(jsval jsvValue, const char *name=NULL);
	};
	typedef JsPtr<Headers> HeadersPtr;

	//////////////////////////////////////////////////////////////////////////
	class HeadersConstructor
		: public JsObject
	{
	public:
		HeadersConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}
#endif
