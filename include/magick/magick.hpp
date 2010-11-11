#ifndef _ccms_magick_magick_hpp_
#define _ccms_magick_magick_hpp_

#include "scripter/jsObject.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Magick
		: public JsObject
	{
	public:
		Magick();
		~Magick();
	};
}}
#endif

