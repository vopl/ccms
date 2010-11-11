#ifndef _ccms_magick_drawableStrokeWidth_hpp_
#define _ccms_magick_drawableStrokeWidth_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeWidth
		: public DrawableBase
	{
	public:
		DrawableStrokeWidth(double width_);
		DrawableStrokeWidth(const DrawableStrokeWidth &from);
		~DrawableStrokeWidth();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_width(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableStrokeWidth &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableStrokeWidth> _impl;

	};
	typedef JsPtr<DrawableStrokeWidth> DrawableStrokeWidthPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeWidthConstructor
		: public JsObject
	{
	public:
		DrawableStrokeWidthConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
