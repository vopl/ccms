#ifndef _ccms_magick_drawableTextAntialias_hpp_
#define _ccms_magick_drawableTextAntialias_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableTextAntialias
		: public DrawableBase
	{
	public:
		DrawableTextAntialias(bool flag_);
		DrawableTextAntialias(const DrawableTextAntialias &from);
		~DrawableTextAntialias();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_flag(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableTextAntialias &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableTextAntialias> _impl;

	};
	typedef JsPtr<DrawableTextAntialias> DrawableTextAntialiasPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableTextAntialiasConstructor
		: public JsObject
	{
	public:
		DrawableTextAntialiasConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
