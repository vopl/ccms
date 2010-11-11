#ifndef _ccms_mime_part_hpp_
#define _ccms_mime_part_hpp_

#include "scripter/jsObject.hpp"

namespace ccms{namespace mime{

	//////////////////////////////////////////////////////////////////////////
	class Headers;
	class Content;
	class File;

	//////////////////////////////////////////////////////////////////////////
	class Part
		: public JsObject
	{
		Headers *_headers;

		union
		{
			Content *_raw;
			File *_file;
			Part *_part;
			JSObject *_array;
		} _content;

		enum EContentType
		{
			ectNull,
			ectRaw,
			ectFile,
			ectPart,
			ectArray,
		} _contentType;

	public:
		Part();
		~Part();

		bool construct(uintN argc, jsval *argv);
		bool jsRegister();

		bool xetter_headers(jsval *vp, bool isGet);
		bool xetter_content(jsval *vp, bool isGet);

		bool call_value(uintN argc, jsval *argv, jsval *rval);

	public:
		bool getFromAll(std::string &data);
		bool getToAll(std::string &data);
		bool getData(std::string &data);

	};
	typedef JsPtr<Part> PartPtr;

	//////////////////////////////////////////////////////////////////////////
	class PartConstructor
		: public JsObject
	{
	public:
		PartConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}
#endif
