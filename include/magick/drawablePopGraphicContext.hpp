#ifndef _ccms_magick_drawablePopGraphicContext_hpp_
#define _ccms_magick_drawablePopGraphicContext_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePopGraphicContext
		: public DrawableBase
	{
	public:
		DrawablePopGraphicContext();
		~DrawablePopGraphicContext();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePopGraphicContext &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePopGraphicContext> _impl;

	};
	typedef JsPtr<DrawablePopGraphicContext> DrawablePopGraphicContextPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePopGraphicContextConstructor
		: public JsObject
	{
	public:
		DrawablePopGraphicContextConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
