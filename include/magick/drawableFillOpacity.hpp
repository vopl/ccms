#ifndef _ccms_magick_drawableFillOpacity_hpp_
#define _ccms_magick_drawableFillOpacity_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableFillOpacity
		: public DrawableBase
	{
	public:
		DrawableFillOpacity(double opacity_);
		DrawableFillOpacity(const DrawableFillOpacity &from);
		~DrawableFillOpacity();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_opacity(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableFillOpacity &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableFillOpacity> _impl;

	};
	typedef JsPtr<DrawableFillOpacity> DrawableFillOpacityPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableFillOpacityConstructor
		: public JsObject
	{
	public:
		DrawableFillOpacityConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
