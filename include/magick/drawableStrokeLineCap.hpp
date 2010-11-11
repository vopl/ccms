#ifndef _ccms_magick_drawableStrokeLineCap_hpp_
#define _ccms_magick_drawableStrokeLineCap_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeLineCap
		: public DrawableBase
	{
	public:
		DrawableStrokeLineCap(::Magick::LineCap from);
		DrawableStrokeLineCap(const DrawableStrokeLineCap &from);
		~DrawableStrokeLineCap();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_lineCap(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableStrokeLineCap &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableStrokeLineCap> _impl;

	};
	typedef JsPtr<DrawableStrokeLineCap> DrawableStrokeLineCapPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeLineCapConstructor
		: public JsObject
	{
	public:
		DrawableStrokeLineCapConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
