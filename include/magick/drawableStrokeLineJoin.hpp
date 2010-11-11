#ifndef _ccms_magick_drawableStrokeLineJoin_hpp_
#define _ccms_magick_drawableStrokeLineJoin_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeLineJoin
		: public DrawableBase
	{
	public:
		DrawableStrokeLineJoin(::Magick::LineJoin from);
		DrawableStrokeLineJoin(const DrawableStrokeLineJoin &from);
		~DrawableStrokeLineJoin();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_lineJoin(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableStrokeLineJoin &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableStrokeLineJoin> _impl;

	};
	typedef JsPtr<DrawableStrokeLineJoin> DrawableStrokeLineJoinPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableStrokeLineJoinConstructor
		: public JsObject
	{
	public:
		DrawableStrokeLineJoinConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
