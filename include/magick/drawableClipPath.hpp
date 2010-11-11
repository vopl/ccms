#ifndef _ccms_magick_drawableClipPath_hpp_
#define _ccms_magick_drawableClipPath_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableClipPath
		: public DrawableBase
	{
	public:
		DrawableClipPath(const char *from);
		DrawableClipPath(const DrawableClipPath &from);
		~DrawableClipPath();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_path(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableClipPath &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableClipPath> _impl;

	};
	typedef JsPtr<DrawableClipPath> DrawableClipPathPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableClipPathConstructor
		: public JsObject
	{
	public:
		DrawableClipPathConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
