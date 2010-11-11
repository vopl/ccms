#ifndef _ccms_magick_drawableCompositeImage_hpp_
#define _ccms_magick_drawableCompositeImage_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableCompositeImage
		: public DrawableBase
	{
	public:
		DrawableCompositeImage(double x_, double y_, const char *filename_);
		DrawableCompositeImage(double x_, double y_, Image *image_);
		DrawableCompositeImage(double x_, double y_, double width_, double height_, const char *filename_);
		DrawableCompositeImage(double x_, double y_, double width_, double height_, Image *image_);
		DrawableCompositeImage(double x_, double y_, double width_, double height_, const char *filename_, ::Magick::CompositeOperator composition_);
		DrawableCompositeImage(double x_, double y_, double width_, double height_, Image *image_, ::Magick::CompositeOperator composition_);
		DrawableCompositeImage(const DrawableCompositeImage& original_);
		~DrawableCompositeImage();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);


		bool xetter_composition(jsval *vp, bool isGet);
		bool xetter_filename(jsval *vp, bool isGet);
		bool xetter_x(jsval *vp, bool isGet);
		bool xetter_y(jsval *vp, bool isGet);
		bool xetter_width(jsval *vp, bool isGet);
		bool xetter_height(jsval *vp, bool isGet);
		bool xetter_image(jsval *vp, bool isGet);
		bool xetter_magick(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableCompositeImage &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableCompositeImage> _impl;
		Image *_image;

	};
	typedef JsPtr<DrawableCompositeImage> DrawableCompositeImagePtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableCompositeImageConstructor
		: public JsObject
	{
	public:
		DrawableCompositeImageConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
