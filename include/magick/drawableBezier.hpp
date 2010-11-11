#ifndef _ccms_magick_drawableBezier_hpp_
#define _ccms_magick_drawableBezier_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableBezier
		: public DrawableBase
	{
	public:
		DrawableBezier(const ::Magick::CoordinateList &list);
		DrawableBezier(const DrawableBezier &from);
		~DrawableBezier();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableBezier &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableBezier> _impl;

	};
	typedef JsPtr<DrawableBezier> DrawableBezierPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableBezierConstructor
		: public JsObject
	{
	public:
		DrawableBezierConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
