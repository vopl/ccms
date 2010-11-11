#ifndef _ccms_magick_drawableViewbox_hpp_
#define _ccms_magick_drawableViewbox_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableViewbox
		: public DrawableBase
	{
	public:
		DrawableViewbox(uint32 x1_, uint32 y1_, uint32 x2_, uint32 y2_);
		DrawableViewbox(const DrawableViewbox &from);
		~DrawableViewbox();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_x1(jsval *vp, bool isGet);
		bool xetter_y1(jsval *vp, bool isGet);
		bool xetter_x2(jsval *vp, bool isGet);
		bool xetter_y2(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableViewbox &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableViewbox> _impl;

	};
	typedef JsPtr<DrawableViewbox> DrawableViewboxPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableViewboxConstructor
		: public JsObject
	{
	public:
		DrawableViewboxConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
