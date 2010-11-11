#ifndef _ccms_magick_drawableCircle_hpp_
#define _ccms_magick_drawableCircle_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableCircle
		: public DrawableBase
	{
	public:
		DrawableCircle(double originX_, double originY_, double perimX_, double perimY_);
		DrawableCircle(const DrawableCircle &from);
		~DrawableCircle();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_originX(jsval *vp, bool isGet);
		bool xetter_originY(jsval *vp, bool isGet);
		bool xetter_perimX(jsval *vp, bool isGet);
		bool xetter_perimY(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableCircle &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableCircle> _impl;

	};
	typedef JsPtr<DrawableCircle> DrawableCirclePtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableCircleConstructor
		: public JsObject
	{
	public:
		DrawableCircleConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
