#ifndef _ccms_magick_drawablePolygon_hpp_
#define _ccms_magick_drawablePolygon_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePolygon
		: public DrawableBase
	{
	public:
		DrawablePolygon(const ::Magick::CoordinateList &list);
		DrawablePolygon(const DrawablePolygon &from);
		~DrawablePolygon();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePolygon &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePolygon> _impl;

	};
	typedef JsPtr<DrawablePolygon> DrawablePolygonPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePolygonConstructor
		: public JsObject
	{
	public:
		DrawablePolygonConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
