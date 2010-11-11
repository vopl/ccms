#ifndef _ccms_magick_drawableMatte_hpp_
#define _ccms_magick_drawableMatte_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableMatte
		: public DrawableBase
	{
	public:
		DrawableMatte(double x_, double y_, ::Magick::PaintMethod paintMethod_);
		DrawableMatte(const DrawableMatte &from);
		~DrawableMatte();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_x(jsval *vp, bool isGet);
		bool xetter_y(jsval *vp, bool isGet);
		bool xetter_paintMethod(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableMatte &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableMatte> _impl;

	};
	typedef JsPtr<DrawableMatte> DrawableMattePtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableMatteConstructor
		: public JsObject
	{
	public:
		DrawableMatteConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
