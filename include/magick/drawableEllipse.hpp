#ifndef _ccms_magick_drawableEllipse_hpp_
#define _ccms_magick_drawableEllipse_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableEllipse
		: public DrawableBase
	{
	public:
		DrawableEllipse(double originX_, double originY_, double radiusX_, double radiusY_, double arcStart_, double arcEnd_);
		DrawableEllipse(const DrawableEllipse &from);
		~DrawableEllipse();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);


		bool xetter_originX(jsval *vp, bool isGet);
		bool xetter_originY(jsval *vp, bool isGet);
		bool xetter_radiusX(jsval *vp, bool isGet);
		bool xetter_radiusY(jsval *vp, bool isGet);
		bool xetter_arcStart(jsval *vp, bool isGet);
		bool xetter_arcEnd(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableEllipse &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableEllipse> _impl;

	};
	typedef JsPtr<DrawableEllipse> DrawableEllipsePtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableEllipseConstructor
		: public JsObject
	{
	public:
		DrawableEllipseConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
