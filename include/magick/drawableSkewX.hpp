#ifndef _ccms_magick_drawableSkewX_hpp_
#define _ccms_magick_drawableSkewX_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableSkewX
		: public DrawableBase
	{
	public:
		DrawableSkewX(double angle_);
		DrawableSkewX(const DrawableSkewX &from);
		~DrawableSkewX();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_angle(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableSkewX &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableSkewX> _impl;

	};
	typedef JsPtr<DrawableSkewX> DrawableSkewXPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableSkewXConstructor
		: public JsObject
	{
	public:
		DrawableSkewXConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
