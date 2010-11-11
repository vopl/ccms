#ifndef _ccms_magick_drawableFillColor_hpp_
#define _ccms_magick_drawableFillColor_hpp_

#include "magick/drawableBase.hpp"
#include "magick/color.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableFillColor
		: public DrawableBase
	{
	public:
		DrawableFillColor();
		DrawableFillColor(Color *from);
		DrawableFillColor(const DrawableFillColor &from);
		~DrawableFillColor();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_color(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableFillColor &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableFillColor> _impl;
	};
	typedef JsPtr<DrawableFillColor> DrawableFillColorPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableFillColorConstructor
		: public JsObject
	{
	public:
		DrawableFillColorConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
