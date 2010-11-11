#ifndef _ccms_magick_drawablePushGraphicContext_hpp_
#define _ccms_magick_drawablePushGraphicContext_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePushGraphicContext
		: public DrawableBase
	{
	public:
		DrawablePushGraphicContext();
		~DrawablePushGraphicContext();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePushGraphicContext &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePushGraphicContext> _impl;

	};
	typedef JsPtr<DrawablePushGraphicContext> DrawablePushGraphicContextPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePushGraphicContextConstructor
		: public JsObject
	{
	public:
		DrawablePushGraphicContextConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
