#ifndef _ccms_magick_drawableStrokeColor_hpp_
#define _ccms_magick_drawableStrokeColor_hpp_

#include "magick/drawableBase.hpp"
#include "magick/color.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeColor
		: public DrawableBase
	{
	public:
		DrawableStrokeColor();
		DrawableStrokeColor(Color *from);
		DrawableStrokeColor(const DrawableStrokeColor &from);
		~DrawableStrokeColor();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_color(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableStrokeColor &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableStrokeColor> _impl;
	};
	typedef JsPtr<DrawableStrokeColor> DrawableStrokeColorPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeColorConstructor
		: public JsObject
	{
	public:
		DrawableStrokeColorConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
