#ifndef _ccms_magick_drawablePopClipPath_hpp_
#define _ccms_magick_drawablePopClipPath_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePopClipPath
		: public DrawableBase
	{
	public:
		DrawablePopClipPath();
		~DrawablePopClipPath();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePopClipPath &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePopClipPath> _impl;

	};
	typedef JsPtr<DrawablePopClipPath> DrawablePopClipPathPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePopClipPathConstructor
		: public JsObject
	{
	public:
		DrawablePopClipPathConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
