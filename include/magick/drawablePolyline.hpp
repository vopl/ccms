#ifndef _ccms_magick_drawablePolyline_hpp_
#define _ccms_magick_drawablePolyline_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePolyline
		: public DrawableBase
	{
	public:
		DrawablePolyline(const ::Magick::CoordinateList &list);
		DrawablePolyline(const DrawablePolyline &from);
		~DrawablePolyline();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePolyline &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePolyline> _impl;

	};
	typedef JsPtr<DrawablePolyline> DrawablePolylinePtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePolylineConstructor
		: public JsObject
	{
	public:
		DrawablePolylineConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
