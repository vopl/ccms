#ifndef _ccms_mime_content_hpp_
#define _ccms_mime_content_hpp_

#include "scripter/jsObject.hpp"

namespace ccms{namespace mime{

	class Content
		: public JsObject
	{
	public:
		Content();
		~Content();

		bool construct(uintN argc, jsval *argv);
		bool jsRegister();

		bool call_value(uintN argc, jsval *argv, jsval *rval);
	};
	typedef JsPtr<Content> ContentPtr;

	//////////////////////////////////////////////////////////////////////////
	class ContentConstructor
		: public JsObject
	{
	public:
		ContentConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}
#endif
