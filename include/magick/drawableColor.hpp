#ifndef _ccms_magick_drawableColor_hpp_
#define _ccms_magick_drawableColor_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableColor
		: public DrawableBase
	{
	public:
		DrawableColor(double x_, double y_, ::Magick::PaintMethod paintMethod_);
		DrawableColor(const DrawableColor &from);
		~DrawableColor();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_x(jsval *vp, bool isGet);
		bool xetter_y(jsval *vp, bool isGet);
		bool xetter_paintMethod(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableColor &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableColor> _impl;

	};
	typedef JsPtr<DrawableColor> DrawableColorPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableColorConstructor
		: public JsObject
	{
	public:
		DrawableColorConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
