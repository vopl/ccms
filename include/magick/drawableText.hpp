#ifndef _ccms_magick_drawableText_hpp_
#define _ccms_magick_drawableText_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableText
		: public DrawableBase
	{
	public:
		DrawableText(double x_, double y_, const char *text);
		DrawableText(const DrawableText &from);
		~DrawableText();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_x(jsval *vp, bool isGet);
		bool xetter_y(jsval *vp, bool isGet);
		bool xetter_text(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableText &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableText> _impl;

	};
	typedef JsPtr<DrawableText> DrawableTextPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableTextConstructor
		: public JsObject
	{
	public:
		DrawableTextConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
