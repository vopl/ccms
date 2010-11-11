#ifndef _ccms_magick_drawableTextUnderColor_hpp_
#define _ccms_magick_drawableTextUnderColor_hpp_

#include "magick/drawableBase.hpp"
#include "magick/color.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableTextUnderColor
		: public DrawableBase
	{
	public:
		DrawableTextUnderColor(Color *from);
		DrawableTextUnderColor(const DrawableTextUnderColor &from);
		~DrawableTextUnderColor();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_color(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableTextUnderColor &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableTextUnderColor> _impl;
	};
	typedef JsPtr<DrawableTextUnderColor> DrawableTextUnderColorPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableTextUnderColorConstructor
		: public JsObject
	{
	public:
		DrawableTextUnderColorConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
