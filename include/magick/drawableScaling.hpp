#ifndef _ccms_magick_drawableScaling_hpp_
#define _ccms_magick_drawableScaling_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableScaling
		: public DrawableBase
	{
	public:
		DrawableScaling(double x_, double y_);
		DrawableScaling(const DrawableScaling &from);
		~DrawableScaling();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_x(jsval *vp, bool isGet);
		bool xetter_y(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableScaling &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableScaling> _impl;

	};
	typedef JsPtr<DrawableScaling> DrawableScalingPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableScalingConstructor
		: public JsObject
	{
	public:
		DrawableScalingConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
