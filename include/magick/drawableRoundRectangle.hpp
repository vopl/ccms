#ifndef _ccms_magick_drawableRoundRectangle_hpp_
#define _ccms_magick_drawableRoundRectangle_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableRoundRectangle
		: public DrawableBase
	{
	public:
		DrawableRoundRectangle(double centerX_, double centerY_, double width_, double hight_, double cornerWidth_, double cornerHeight_);
		DrawableRoundRectangle(const DrawableRoundRectangle &from);
		~DrawableRoundRectangle();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_centerX(jsval *vp, bool isGet);
		bool xetter_centerY(jsval *vp, bool isGet);
		bool xetter_width(jsval *vp, bool isGet);
		bool xetter_height(jsval *vp, bool isGet);
		bool xetter_cornerWidth(jsval *vp, bool isGet);
		bool xetter_cornerHeight(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableRoundRectangle &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableRoundRectangle> _impl;

	};
	typedef JsPtr<DrawableRoundRectangle> DrawableRoundRectanglePtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableRoundRectangleConstructor
		: public JsObject
	{
	public:
		DrawableRoundRectangleConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
