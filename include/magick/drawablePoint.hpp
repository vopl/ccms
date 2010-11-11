#ifndef _ccms_magick_drawablePoint_hpp_
#define _ccms_magick_drawablePoint_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePoint
		: public DrawableBase
	{
	public:
		DrawablePoint(double x_, double y_);
		DrawablePoint(const DrawablePoint &from);
		~DrawablePoint();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_x(jsval *vp, bool isGet);
		bool xetter_y(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePoint &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePoint> _impl;

	};
	typedef JsPtr<DrawablePoint> DrawablePointPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePointConstructor
		: public JsObject
	{
	public:
		DrawablePointConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
