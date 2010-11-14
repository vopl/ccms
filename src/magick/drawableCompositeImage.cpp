#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableCompositeImage.hpp"
#include "magick/image.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableCompositeImage::DrawableCompositeImage(double x_, double y_, const char *filename_)
		: DrawableBase("DrawableCompositeImage")
		, _impl(new ::Magick::DrawableCompositeImage(x_, y_, filename_))
		, _image(NULL)
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, 0, JSVAL_NULL);
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableCompositeImage::DrawableCompositeImage(double x_, double y_, Image *image_)
		: DrawableBase("DrawableCompositeImage")
		, _impl(new ::Magick::DrawableCompositeImage(x_, y_, image_->getImpl()))
		, _image(image_)
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, 0, _image->thisJsval());
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableCompositeImage::DrawableCompositeImage(double x_, double y_, double width_, double height_, const char *filename_)
		: DrawableBase("DrawableCompositeImage")
		, _impl(new ::Magick::DrawableCompositeImage(x_, y_, width_, height_, filename_))
		, _image(NULL)
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, 0, JSVAL_NULL);
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableCompositeImage::DrawableCompositeImage(double x_, double y_, double width_, double height_, Image *image_)
		: DrawableBase("DrawableCompositeImage")
		, _impl(new ::Magick::DrawableCompositeImage(x_, y_, width_, height_, image_->getImpl()))
		, _image(image_)
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, 0, _image->thisJsval());
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableCompositeImage::DrawableCompositeImage(double x_, double y_, double width_, double height_, const char *filename_, ::Magick::CompositeOperator composition_)
		: DrawableBase("DrawableCompositeImage")
		, _impl(new ::Magick::DrawableCompositeImage(x_, y_, width_, height_, filename_, composition_))
		, _image(NULL)
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, 0, JSVAL_NULL);
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableCompositeImage::DrawableCompositeImage(double x_, double y_, double width_, double height_, Image *image_, ::Magick::CompositeOperator composition_)
		: DrawableBase("DrawableCompositeImage")
		, _impl(new ::Magick::DrawableCompositeImage(x_, y_, width_, height_, image_->getImpl(), composition_))
		, _image(image_)
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, 0, _image->thisJsval());
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableCompositeImage::DrawableCompositeImage(const DrawableCompositeImage &from)
		: DrawableBase("DrawableCompositeImage")
		, _impl(new ::Magick::DrawableCompositeImage(from.getImpl()))
		, _image(from._image)
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, 0, JSVAL_NULL);
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableCompositeImage::~DrawableCompositeImage()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableCompositeImage(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::xetter_composition(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->composition());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->composition((::Magick::CompositeOperator)v);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::xetter_filename(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			std::string str = _impl->filename();
			*vp = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, str.data(), str.size()));
		}
		else
		{
			char *str;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &str)) return false;
			_impl->filename(str);
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::xetter_x(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->x(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->x(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::xetter_y(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->y(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->y(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::xetter_width(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->width(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->width(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::xetter_height(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->height(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->height(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::xetter_image(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(_image)
			{
				*vp = _image->thisJsval();
			}
			else
			{
				*vp = JSVAL_NULL;
			}
		}
		else
		{
			if(_image)
			{
				_image = NULL;
			}
			_image = jsobjFromJsv<Image>(*vp);
			if(_image)
			{
				if(!JS_SetReservedSlot(ecx()->_jsCx, _js, 0, _image->thisJsval())) return false;
				IMTRY()
					_impl->image(_image->getImpl());
				IMCATCH(return false);
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "DrawableCompositeImage.image must be a Image object");
				return false;

				//if(!JS_SetReservedSlot(ecx()->_jsCx, _js, 0, JSVAL_NULL)) return false;
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::xetter_magick(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			std::string str = _impl->magick();
			*vp = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, str.data(), str.size()));
		}
		else
		{
			char *str;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &str)) return false;
			IMTRY()
				_impl->magick(str);
			IMCATCH(return false);
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableCompositeImage &DrawableCompositeImage::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableCompositeImage::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableCompositeImage::call_copy, 0))return false;


		if(!jsRegisterProp("composition", &DrawableCompositeImage::xetter_composition, false)) return false;
		if(!jsRegisterProp("filename", &DrawableCompositeImage::xetter_filename, false)) return false;
		if(!jsRegisterProp("x", &DrawableCompositeImage::xetter_x, false)) return false;
		if(!jsRegisterProp("y", &DrawableCompositeImage::xetter_y, false)) return false;
		if(!jsRegisterProp("width", &DrawableCompositeImage::xetter_width, false)) return false;
		if(!jsRegisterProp("height", &DrawableCompositeImage::xetter_height, false)) return false;
		if(!jsRegisterProp("image", &DrawableCompositeImage::xetter_image, false)) return false;
		if(!jsRegisterProp("magick", &DrawableCompositeImage::xetter_magick, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableCompositeImageConstructor::DrawableCompositeImageConstructor()
		: JsObject(true, "DrawableCompositeImageConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableCompositeImageConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableCompositeImage *from = jsobjFromJsv<DrawableCompositeImage>(argv[0]);
			if(from)
			{
				*rval = (new DrawableCompositeImage(*from))->thisJsval();
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "DrawableCompositeImageConstructor 1st arg must be a DrawableCompositeImage object");
				return false;
			}
			
		}
		else if(3 == argc)
		{
			//DrawableCompositeImage(double x_, double y_, const char *filename_);
			//DrawableCompositeImage(double x_, double y_, const Image *image_);

			double x_;
			double y_;
			Image *image_;
			const char *filename_;

			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &x_, &y_)) return false;
			image_ = jsobjFromJsv<Image>(argv[2]);
			if(image_)
			{
				*rval = (new DrawableCompositeImage(x_, y_, image_))->thisJsval();
			}
			else
			{
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+2, "s", &filename_)) return false;
				*rval = (new DrawableCompositeImage(x_, y_, filename_))->thisJsval();
			}
		}
		else if(5 == argc)
		{
			//DrawableCompositeImage(double x_, double y_, double width_, double height_, const char *filename_);
			//DrawableCompositeImage(double x_, double y_, double width_, double height_, const Image *image_);

			double x_;
			double y_;
			double width_;
			double height_;
			Image *image_;
			const char *filename_;

			if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "dddd", &x_, &y_, &width_, &height_)) return false;
			image_ = jsobjFromJsv<Image>(argv[4]);
			if(image_)
			{
				*rval = (new DrawableCompositeImage(x_, y_, width_, height_, image_))->thisJsval();
			}
			else
			{
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+4, "s", &filename_)) return false;
				*rval = (new DrawableCompositeImage(x_, y_, width_, height_, filename_))->thisJsval();
			}
		}
		else if(6 == argc)
		{
			//DrawableCompositeImage(double x_, double y_, double width_, double height_, const char *filename_, CompositeOperator composition_);
			//DrawableCompositeImage(double x_, double y_, double width_, double height_, const Image *image_, CompositeOperator composition_);

			jsdouble x_;
			jsdouble y_;
			jsdouble width_;
			jsdouble height_;
			Image *image_;
			const char *filename_;
			uint32 composition_;

			if(!JS_ConvertArguments(ecx()->_jsCx, 5, argv, "dddd*u", &x_, &y_, &width_, &height_, &composition_)) return false;
			image_ = jsobjFromJsv<Image>(argv[4]);
			if(image_)
			{
				*rval = (new DrawableCompositeImage(x_, y_, width_, height_, image_, (::Magick::CompositeOperator)composition_))->thisJsval();
			}
			else
			{
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+4, "s", &filename_)) return false;
				*rval = (new DrawableCompositeImage(x_, y_, width_, height_, filename_, (::Magick::CompositeOperator)composition_))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "CompositeImageConstructor: must be called with 1 or 6 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
