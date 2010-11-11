#ifndef _ccms_magick_drawableDashArray_hpp_
#define _ccms_magick_drawableDashArray_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableDashArray
		: public DrawableBase
	{
	public:
		DrawableDashArray(const double* dasharray_);
		DrawableDashArray(const DrawableDashArray &from);
		~DrawableDashArray();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_dasharray(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableDashArray &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableDashArray> _impl;

	};
	typedef JsPtr<DrawableDashArray> DrawableDashArrayPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableDashArrayConstructor
		: public JsObject
	{
	public:
		DrawableDashArrayConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
