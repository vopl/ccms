#ifndef _ccms_mime_headerComplex_hpp_
#define _ccms_mime_headerComplex_hpp_

#include "mime/headerBase.hpp"

namespace ccms{namespace mime{

	class HeaderComplex
		: public HeaderBase
	{
	public:
		HeaderComplex();
		~HeaderComplex();

		bool construct(uintN argc, jsval *argv);
		bool jsRegister();
	};
	typedef JsPtr<HeaderComplex> HeaderComplexPtr;


	//////////////////////////////////////////////////////////////////////////
	class HeaderComplexConstructor
		: public JsObject
	{
	public:
		HeaderComplexConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}
#endif
