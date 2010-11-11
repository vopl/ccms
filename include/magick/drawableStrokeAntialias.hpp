#ifndef _ccms_magick_drawableStrokeAntialias_hpp_
#define _ccms_magick_drawableStrokeAntialias_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeAntialias
		: public DrawableBase
	{
	public:
		DrawableStrokeAntialias(bool flag_);
		DrawableStrokeAntialias(const DrawableStrokeAntialias &from);
		~DrawableStrokeAntialias();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_flag(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableStrokeAntialias &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableStrokeAntialias> _impl;

	};
	typedef JsPtr<DrawableStrokeAntialias> DrawableStrokeAntialiasPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeAntialiasConstructor
		: public JsObject
	{
	public:
		DrawableStrokeAntialiasConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
