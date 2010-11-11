#ifndef _ccms_magick_drawablePushClipPath_hpp_
#define _ccms_magick_drawablePushClipPath_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePushClipPath
		: public DrawableBase
	{
	public:
		DrawablePushClipPath(const char *id_);
		DrawablePushClipPath(const DrawablePushClipPath &from);
		~DrawablePushClipPath();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

// 		bool xetter_id(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePushClipPath &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePushClipPath> _impl;

	};
	typedef JsPtr<DrawablePushClipPath> DrawablePushClipPathPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePushClipPathConstructor
		: public JsObject
	{
	public:
		DrawablePushClipPathConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
