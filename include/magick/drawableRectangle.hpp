#ifndef _ccms_magick_drawableRectangle_hpp_
#define _ccms_magick_drawableRectangle_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableRectangle
		: public DrawableBase
	{
	public:
		DrawableRectangle(double upperLeftX_, double upperLeftY_, double lowerRightX_, double lowerRightY_);
		DrawableRectangle(const DrawableRectangle &from);
		~DrawableRectangle();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_upperLeftX(jsval *vp, bool isGet);
		bool xetter_upperLeftY(jsval *vp, bool isGet);
		bool xetter_lowerRightX(jsval *vp, bool isGet);
		bool xetter_lowerRightY(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableRectangle &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableRectangle> _impl;

	};
	typedef JsPtr<DrawableRectangle> DrawableRectanglePtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableRectangleConstructor
		: public JsObject
	{
	public:
		DrawableRectangleConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
