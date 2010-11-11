#ifndef _ccms_mime_file_hpp_
#define _ccms_mime_file_hpp_

#include "scripter/jsObject.hpp"

namespace ccms{namespace mime{

	class File
		: public JsObject
	{
	public:
		File();
		~File();

		bool construct(uintN argc, jsval *argv);
		bool jsRegister();

		bool call_value(uintN argc, jsval *argv, jsval *rval);
	};
	typedef JsPtr<File> FilePtr;

	//////////////////////////////////////////////////////////////////////////
	class FileConstructor
		: public JsObject
	{
	public:
		FileConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}
#endif
