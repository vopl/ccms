#ifndef _ccms_magick_drawableStrokeOpacity_hpp_
#define _ccms_magick_drawableStrokeOpacity_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeOpacity
		: public DrawableBase
	{
	public:
		DrawableStrokeOpacity(double opacity_);
		DrawableStrokeOpacity(const DrawableStrokeOpacity &from);
		~DrawableStrokeOpacity();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_opacity(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableStrokeOpacity &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableStrokeOpacity> _impl;

	};
	typedef JsPtr<DrawableStrokeOpacity> DrawableStrokeOpacityPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeOpacityConstructor
		: public JsObject
	{
	public:
		DrawableStrokeOpacityConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
