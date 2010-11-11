#ifndef _ccms_magick_drawableSkewY_hpp_
#define _ccms_magick_drawableSkewY_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableSkewY
		: public DrawableBase
	{
	public:
		DrawableSkewY(double angle_);
		DrawableSkewY(const DrawableSkewY &from);
		~DrawableSkewY();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_angle(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableSkewY &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableSkewY> _impl;

	};
	typedef JsPtr<DrawableSkewY> DrawableSkewYPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableSkewYConstructor
		: public JsObject
	{
	public:
		DrawableSkewYConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
