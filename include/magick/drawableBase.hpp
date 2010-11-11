#ifndef _ccms_magick_drawableBase_hpp_
#define _ccms_magick_drawableBase_hpp_

#include "scripter/jsObject.hpp"

namespace ccms{namespace magick{

	class DrawableBase
		: public JsObject
	{
	public:
		DrawableBase(const char *strVal)
			: JsObject(true, strVal)
		{
		}
		virtual ~DrawableBase(){};
		virtual ::Magick::Drawable getBaseImpl() =0;
	};
}}

#endif
