#ifndef _ccms_magick_pathBase_hpp_
#define _ccms_magick_pathBase_hpp_

#include "scripter/jsObject.hpp"

namespace ccms{namespace magick{

	class PathBase
		: public JsObject
	{
	public:
		PathBase(const char *strVal)
			: JsObject(true, strVal)
		{
		}
		virtual ~PathBase(){};
		virtual ::Magick::VPathBase &getBaseImpl() =0;
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}

#endif
