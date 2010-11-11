#ifndef _ccms_magick_drawableFont_hpp_
#define _ccms_magick_drawableFont_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableFont
		: public DrawableBase
	{
	public:

		DrawableFont(const char *font_);
		DrawableFont(const char *family_, ::Magick::StyleType style_, uint32 weight_, ::Magick::StretchType stretch_);
		DrawableFont(const DrawableFont &from);
		~DrawableFont();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_font(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableFont &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableFont> _impl;

	};
	typedef JsPtr<DrawableFont> DrawableFontPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableFontConstructor
		: public JsObject
	{
	public:
		DrawableFontConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
