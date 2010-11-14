#include "magick_pch.h"
#include "magick/imageAttributes.hpp"
#include "magick/image.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/imageCnvtHelpers.hpp"

namespace ccms{namespace magick{

	//////////////////////////////////////////////////////////////////////////
	ImageAttributes::ImageAttributes(Image *img)
		: JsObject(true, "ImageAttributes")
		, _img(img)
	{
		(JSExceptionReporter)JS_SetReservedSlot(ecx()->_jsCx, _js, 0, img->thisJsval());

		(JSExceptionReporter)jsRegister();
	}
	
	//////////////////////////////////////////////////////////////////////////
	ImageAttributes::~ImageAttributes()
	{

	}








	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_image(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = _img->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_adjoin(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool flag;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &flag)) return false;
			IMTRY()
				iimpl()->adjoin(flag);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			flag = iimpl()->adjoin();
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(flag);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_antiAlias(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool flag;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &flag)) return false;
			IMTRY()
				iimpl()->antiAlias(flag);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			flag = iimpl()->antiAlias();
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(flag);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_animationDelay(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->animationDelay(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->animationDelay();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_animationIterations(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->animationIterations(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->animationIterations();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	bool ImageAttributes::call_attribute(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "Image.attribute must be called with 1 or 2 args");
			return false;
		}

		char *name;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &name)) return false;


		if(argc>1)
		{
			char *value;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "s", &value)) return false;
			IMTRY()
				iimpl()->attribute(name, value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		std::string value;
		IMTRY()
			value = iimpl()->attribute(name);
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_backgroundColor(uintN argc, jsval *argv, jsval *rval)
	{
		::Magick::Color value;

		if(argc)
		{
			boost::shared_ptr< ::Magick::Color> value;
			if(!impl::colorFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.backgroundColor 1st arg must be Color");
				return false;
			}
			IMTRY()
				iimpl()->backgroundColor(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->backgroundColor();
		IMCATCH(return false);

		*rval = (new Color(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_backgroundTexture(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			char *value;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value)) return false;
			IMTRY()
				iimpl()->backgroundTexture(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		std::string value;
		IMTRY()
			value = iimpl()->backgroundTexture();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_baseColumns(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;
		IMTRY()
			value = iimpl()->baseColumns();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_baseFilename(uintN argc, jsval *argv, jsval *rval)
	{
		std::string value;
		IMTRY()
			value = iimpl()->baseFilename();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_baseRows(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;
		IMTRY()
			value = iimpl()->baseRows();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_borderColor(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			boost::shared_ptr< ::Magick::Color> value;
			if(!impl::colorFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.borderColor 1st arg must be Color");
				return false;
			}
			IMTRY()
				iimpl()->borderColor(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		::Magick::Color value;
		IMTRY()
			value = iimpl()->borderColor();
		IMCATCH(return false);

		*rval = (new Color(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_boundingBox(uintN argc, jsval *argv, jsval *rval)
	{
		::Magick::Geometry value;
		IMTRY()
			value = iimpl()->boundingBox();
		IMCATCH(return false);

		*rval = (new Geometry(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_boxColor(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			boost::shared_ptr< ::Magick::Color> value;
			if(!impl::colorFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.boxColor 1st arg must be Color");
				return false;
			}
			IMTRY()
				iimpl()->boxColor(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		::Magick::Color value;
		IMTRY()
			value = iimpl()->boxColor();
		IMCATCH(return false);

		*rval = (new Color(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_chromaBluePrimary(uintN argc, jsval *argv, jsval *rval)
	{
		if(0!=argc && 2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.chromaBluePrimary must be called with 0 or 2 args");
			return false;
		}

		if(2 == argc)
		{
			jsdouble x,y;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &x, &y)) return false;
			IMTRY()
				iimpl()->chromaBluePrimary(x, y);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		jsdouble x,y;
		IMTRY()
			iimpl()->chromaBluePrimary(&x, &y);
		IMCATCH(return false);

		jsval jsva[2];
		if(!JS_NewDoubleValue(ecx()->_jsCx, x, &jsva[0])) return false;
		if(!JS_NewDoubleValue(ecx()->_jsCx, y, &jsva[1])) return false;

		*rval = OBJECT_TO_JSVAL(JS_NewArrayObject(ecx()->_jsCx, 2, jsva));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_chromaGreenPrimary(uintN argc, jsval *argv, jsval *rval)
	{
		if(0!=argc && 2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.chromaGreenPrimary must be called with 0 or 2 args");
			return false;
		}

		if(2 == argc)
		{
			jsdouble x,y;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &x, &y)) return false;
			IMTRY()
				iimpl()->chromaGreenPrimary(x, y);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		jsdouble x,y;
		IMTRY()
			iimpl()->chromaGreenPrimary(&x, &y);
		IMCATCH(return false);

		jsval jsva[2];
		if(!JS_NewDoubleValue(ecx()->_jsCx, x, &jsva[0])) return false;
		if(!JS_NewDoubleValue(ecx()->_jsCx, y, &jsva[1])) return false;

		*rval = OBJECT_TO_JSVAL(JS_NewArrayObject(ecx()->_jsCx, 2, jsva));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_chromaRedPrimary(uintN argc, jsval *argv, jsval *rval)
	{
		if(0!=argc && 2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.chromaRedPrimary must be called with 0 or 2 args");
			return false;
		}

		if(2 == argc)
		{
			jsdouble x,y;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &x, &y)) return false;
			IMTRY()
				iimpl()->chromaRedPrimary(x, y);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		jsdouble x,y;
		IMTRY()
			iimpl()->chromaRedPrimary(&x, &y);
		IMCATCH(return false);

		jsval jsva[2];
		if(!JS_NewDoubleValue(ecx()->_jsCx, x, &jsva[0])) return false;
		if(!JS_NewDoubleValue(ecx()->_jsCx, y, &jsva[1])) return false;

		*rval = OBJECT_TO_JSVAL(JS_NewArrayObject(ecx()->_jsCx, 2, jsva));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_chromaWhitePoint(uintN argc, jsval *argv, jsval *rval)
	{
		if(0!=argc && 2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.chromaWhitePoint must be called with 0 or 2 args");
			return false;
		}

		if(2 == argc)
		{
			jsdouble x,y;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &x, &y)) return false;
			IMTRY()
				iimpl()->chromaWhitePoint(x, y);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		jsdouble x,y;
		IMTRY()
			iimpl()->chromaWhitePoint(&x, &y);
		IMCATCH(return false);

		jsval jsva[2];
		if(!JS_NewDoubleValue(ecx()->_jsCx, x, &jsva[0])) return false;
		if(!JS_NewDoubleValue(ecx()->_jsCx, y, &jsva[1])) return false;

		*rval = OBJECT_TO_JSVAL(JS_NewArrayObject(ecx()->_jsCx, 2, jsva));
		return true;
	}

	bool ImageAttributes::call_classType(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->classType((::Magick::ClassType)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->classType();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_clipMask(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			Image *image = impl::imageFromJsv(argv[0]);
			if(!image)
			{
				JS_ReportError(ecx()->_jsCx, "Image.clipMask 1st arg must be Image");
				return false;
			}

			IMTRY()
				iimpl()->clipMask(image->getImpl());
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		Image *image;
		IMTRY()
			image = new Image(iimpl()->clipMask());
		IMCATCH(return false);

		*rval = image->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_colorFuzz(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &value)) return false;
			IMTRY()
				iimpl()->colorFuzz(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->colorFuzz();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_colorMap(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "Image.colorMap must be called with 1 or 2 args");
			return false;
		}

		uint32 index;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &index)) return false;


		if(argc>1)
		{
			boost::shared_ptr< ::Magick::Color> color;
			if(!impl::colorFromJsv(argv[1], color)) return false;
			if(!color)
			{
				JS_ReportError(ecx()->_jsCx, "Image.colorMap 1st arg must be Color");
				return false;
			}

			IMTRY()
				iimpl()->colorMap(index, *color);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		::Magick::Color value;
		IMTRY()
			value = iimpl()->colorMap(index);
		IMCATCH(return false);

		*rval = (new Color(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_colorMapSize(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->colorMapSize(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->colorMapSize();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_colorSpace(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->colorSpace((::Magick::ColorspaceType)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->colorSpace();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_colorspaceType(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->colorspaceType((::Magick::ColorspaceType)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->colorspaceType();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	bool ImageAttributes::call_columns(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;
		IMTRY()
			value = iimpl()->columns();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_compressType(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->compressType((::Magick::CompressionType)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->compressType();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_debug(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &value)) return false;
			IMTRY()
				iimpl()->debug(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->debug();
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_defineValue(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 2)
		{
			JS_ReportError(ecx()->_jsCx, "Image.defineValue must be called with 2 or 3 args");
			return false;
		}

		char *magick, *key;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &magick, &key)) return false;

		if(argc>1)
		{
			char *value;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+2, "s", &value)) return false;
			IMTRY()
				iimpl()->defineValue(magick, key, value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		std::string value;
		IMTRY()
			value = iimpl()->defineValue(magick, key);
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	bool ImageAttributes::call_defineSet(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 2)
		{
			JS_ReportError(ecx()->_jsCx, "Image.defineSet must be called with 2 or 3 args");
			return false;
		}

		char *magick, *key;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &magick, &key)) return false;

		if(argc>1)
		{
			JSBool value;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+2, "b", &value)) return false;
			IMTRY()
				iimpl()->defineSet(magick, key, value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		JSBool value;
		IMTRY()
			value = iimpl()->defineSet(magick, key);
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_density(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			boost::shared_ptr< ::Magick::Geometry> value;
			if(!impl::geometryFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.density 1st arg must be Geometry");
				return false;
			}
			IMTRY()
				iimpl()->density(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		::Magick::Geometry value;
		IMTRY()
			value = iimpl()->density();
		IMCATCH(return false);

		*rval = (new Geometry(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_depth(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->depth(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->depth();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_directory(uintN argc, jsval *argv, jsval *rval)
	{
		std::string value;
		IMTRY()
			value = iimpl()->directory();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_endian(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->endian((::Magick::EndianType)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->endian();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_exifProfile(uintN argc, jsval *argv, jsval *rval)
	{
		JS_ReportError(ecx()->_jsCx, "Image.exifProfile not implemented");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_fileName(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			char *value;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value)) return false;
			IMTRY()
				iimpl()->fileName(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}
		std::string value;
		IMTRY()
			value = iimpl()->fileName();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_fileSize(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;
		IMTRY()
			value = iimpl()->endian();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_fillColor(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			boost::shared_ptr< ::Magick::Color> value;
			if(!impl::colorFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.fillColor 1st arg must be Color");
				return false;
			}
			IMTRY()
				iimpl()->fillColor(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		::Magick::Color value;
		IMTRY()
			value = iimpl()->fillColor();
		IMCATCH(return false);

		*rval = (new Color(value))->thisJsval();
		return true;
	}

	bool ImageAttributes::call_fillRule(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->fillRule((::Magick::FillRule)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->fillRule();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	bool ImageAttributes::call_fillPattern(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			Image *image = impl::imageFromJsv(argv[0]);
			if(!image)
			{
				JS_ReportError(ecx()->_jsCx, "Image.fillPattern 1st arg must be Image");
				return false;
			}

			IMTRY()
				iimpl()->fillPattern(image->getImpl());
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		Image *image;
		IMTRY()
			image = new Image(iimpl()->fillPattern());
		IMCATCH(return false);

		*rval = image->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_filterType(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->filterType((::Magick::FilterTypes)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->filterType();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_font(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			char *value;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value)) return false;
			IMTRY()
				iimpl()->font(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}
		std::string value;
		IMTRY()
			value = iimpl()->font();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_fontPointsize(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &value)) return false;
			IMTRY()
				iimpl()->fontPointsize(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->fontPointsize();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	bool ImageAttributes::call_fontTypeMetrics(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.fontTypeMetrics must be called with 1 arg");
			return false;
		}

		char *text;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &text)) return false;

		::Magick::TypeMetric tm;
		IMTRY()
			iimpl()->fontTypeMetrics(text, &tm);
		IMCATCH(return false);

		JSObject *obj;
		if(!(obj = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL))) return false;

		jsval jsv;

		if(!JS_NewDoubleValue(ecx()->_jsCx, tm.ascent(), &jsv)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, obj, "ascent", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;

		if(!JS_NewDoubleValue(ecx()->_jsCx, tm.descent(), &jsv)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, obj, "descent", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;

		if(!JS_NewDoubleValue(ecx()->_jsCx, tm.maxHorizontalAdvance(), &jsv)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, obj, "maxHorizontalAdvance", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;

		if(!JS_NewDoubleValue(ecx()->_jsCx, tm.textHeight(), &jsv)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, obj, "textHeight", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;

		if(!JS_NewDoubleValue(ecx()->_jsCx, tm.textWidth(), &jsv)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, obj, "textWidth", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;

		*rval = OBJECT_TO_JSVAL(obj);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_format(uintN argc, jsval *argv, jsval *rval)
	{
		std::string value;
		IMTRY()
			value = iimpl()->format();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_geometry(uintN argc, jsval *argv, jsval *rval)
	{
		::Magick::Geometry value;
		IMTRY()
			value = iimpl()->geometry();
		IMCATCH(return false);

		*rval = (new Geometry(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_gifDisposeMethod(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->gifDisposeMethod(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->gifDisposeMethod();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_iccColorProfile(uintN argc, jsval *argv, jsval *rval)
	{
		JS_ReportError(ecx()->_jsCx, "Image.iccColorProfile not implemented");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_interlaceType(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->interlaceType((::Magick::InterlaceType)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->interlaceType();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_iptcProfile(uintN argc, jsval *argv, jsval *rval)
	{
		JS_ReportError(ecx()->_jsCx, "Image.iptcProfile not implemented");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_isValid(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &value)) return false;
			IMTRY()
				iimpl()->isValid(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->isValid();
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_lineWidth(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &value)) return false;
			IMTRY()
				iimpl()->lineWidth(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->lineWidth();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_magick(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			char *value;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value)) return false;
			IMTRY()
				iimpl()->magick(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}
		std::string value;
		IMTRY()
			value = iimpl()->magick();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_matte(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &value)) return false;
			IMTRY()
				iimpl()->matte(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->matte();
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_matteColor(uintN argc, jsval *argv, jsval *rval)
	{
		::Magick::Color value;

		if(argc)
		{
			boost::shared_ptr< ::Magick::Color> value;
			if(!impl::colorFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.matteColor 1st arg must be Color");
				return false;
			}
			IMTRY()
				iimpl()->matteColor(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->matteColor();
		IMCATCH(return false);

		*rval = (new Color(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_meanErrorPerPixel(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		IMTRY()
			value = iimpl()->meanErrorPerPixel();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_modulusDepth(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->modulusDepth(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->modulusDepth();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_montageGeometry(uintN argc, jsval *argv, jsval *rval)
	{
		::Magick::Geometry value;
		IMTRY()
			value = iimpl()->montageGeometry();
		IMCATCH(return false);

		*rval = (new Geometry(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_monochrome(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &value)) return false;
			IMTRY()
				iimpl()->monochrome(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->monochrome();
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_normalizedMaxError(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		IMTRY()
			value = iimpl()->normalizedMaxError();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_normalizedMeanError(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		IMTRY()
			value = iimpl()->normalizedMeanError();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_orientation(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->orientation((::Magick::OrientationType)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->orientation();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_page(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			boost::shared_ptr< ::Magick::Geometry> value;
			if(!impl::geometryFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.page 1st arg must be Geometry");
				return false;
			}
			IMTRY()
				iimpl()->page(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		::Magick::Geometry value;
		IMTRY()
			value = iimpl()->page();
		IMCATCH(return false);

		*rval = (new Geometry(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_penColor(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			boost::shared_ptr< ::Magick::Color> value;
			if(!impl::colorFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.penColor 1st arg must be Color");
				return false;
			}
			IMTRY()
				iimpl()->penColor(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		::Magick::Color value;
		IMTRY()
			value = iimpl()->penColor();
		IMCATCH(return false);

		*rval = (new Color(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_penTexture(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			Image *image = impl::imageFromJsv(argv[0]);
			if(!image)
			{
				JS_ReportError(ecx()->_jsCx, "Image.penTexture 1st arg must be Image");
				return false;
			}

			IMTRY()
				iimpl()->penTexture(image->getImpl());
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		Image *image;
		IMTRY()
			image = new Image(iimpl()->penTexture());
		IMCATCH(return false);

		*rval = image->thisJsval();
		return true;
	}

	bool ImageAttributes::call_pixelColor(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 2)
		{
			JS_ReportError(ecx()->_jsCx, "Image.pixelColor must be called with 2 or 3 args");
			return false;
		}

		int32 x,y;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ii", &x, &y)) return false;
		argc-=2;
		argv+=2;

		::Magick::Color value;

		if(argc)
		{
			boost::shared_ptr< ::Magick::Color> value;
			if(!impl::colorFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.pixelColor 3rd arg must be Color");
				return false;
			}
			IMTRY()
				iimpl()->pixelColor(x,y,*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->pixelColor(x,y);
		IMCATCH(return false);

		*rval = (new Color(value))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_profile(uintN argc, jsval *argv, jsval *rval)
	{
		JS_ReportError(ecx()->_jsCx, "Image.profile not implemented");
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_quality(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->quality(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->quality();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_quantizeColors(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->quantizeColors(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->quantizeColors();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_quantizeColorSpace(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->quantizeColorSpace((::Magick::ColorspaceType)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->quantizeColorSpace();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_quantizeDither(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &value)) return false;
			IMTRY()
				iimpl()->quantizeDither(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->quantizeDither();
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_quantizeTreeDepth(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->quantizeTreeDepth(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->quantizeTreeDepth();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_renderingIntent(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->renderingIntent((::Magick::RenderingIntent)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->renderingIntent();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_resolutionUnits(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->resolutionUnits((::Magick::ResolutionType)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->resolutionUnits();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_rows(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		IMTRY()
			value = iimpl()->rows();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_scene(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->scene(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->scene();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_signature(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool force = false;
		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &force)) return false;
		}

		std::string signature;
		IMTRY()
			signature = iimpl()->signature(force);
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, signature.data(), signature.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_size(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			boost::shared_ptr< ::Magick::Geometry> value;
			if(!impl::geometryFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.size 1st arg must be Geometry");
				return false;
			}
			IMTRY()
				iimpl()->size(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		::Magick::Geometry value;
		IMTRY()
			value = iimpl()->size();
		IMCATCH(return false);

		*rval = (new Geometry(value))->thisJsval();
		return true;
	}

	bool ImageAttributes::call_strokeAntiAlias(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &value)) return false;
			IMTRY()
				iimpl()->strokeAntiAlias(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->strokeAntiAlias();
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_strokeColor(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			boost::shared_ptr< ::Magick::Color> value;
			if(!impl::colorFromJsv(argv[0], value)) return false;
			if(!value)
			{
				JS_ReportError(ecx()->_jsCx, "Image.strokeColor 1st arg must be Color");
				return false;
			}
			IMTRY()
				iimpl()->strokeColor(*value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		::Magick::Color value;
		IMTRY()
			value = iimpl()->strokeColor();
		IMCATCH(return false);

		*rval = (new Color(value))->thisJsval();
		return true;
	}

	bool ImageAttributes::call_strokeDashArray(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			if(!JSVAL_IS_OBJECT(*argv) || JSVAL_IS_NULL(*argv))
			{
				JS_ReportError(ecx()->_jsCx, "Image.strokeDashArray 1st arg must be a Array of numbers");
				return false;
			}
			JSObject *arrobj = JSVAL_TO_OBJECT(*argv);
			if(!JS_IsArrayObject(ecx()->_jsCx, arrobj))
			{
				JS_ReportError(ecx()->_jsCx, "Image.strokeDashArray 1st arg must be a Array of numbers");
				return false;
			}

			jsuint len;
			JS_GetArrayLength(ecx()->_jsCx, arrobj, &len);

			std::vector<jsdouble> dvals(len+1);
			for(jsuint i(0); i<len; i++)
			{
				jsval jsv;
				if(!JS_GetElement(ecx()->_jsCx, arrobj, i, &jsv)) return false;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "d", &dvals[i])) return false;
			}

			IMTRY()
				iimpl()->strokeDashArray(&dvals[0]);
			IMCATCH(return false);

			*rval = JSVAL_VOID;
			return true;
		}

		const double *dvals;
		IMTRY()
			dvals = iimpl()->strokeDashArray();
		IMCATCH(return false);

		std::vector<jsval> jsvals;
		while(*dvals)
		{
			jsval jsv;
			if(!JS_NewDoubleValue(ecx()->_jsCx, *dvals, &jsv)) return false;

			jsvals.push_back(jsv);
			dvals++;
		}

		if(jsvals.size())
		{
			JSObject *obj = JS_NewArrayObject(ecx()->_jsCx, jsvals.size(), &jsvals[0]);
			*rval = OBJECT_TO_JSVAL(obj);
		}
		else
		{
			JSObject *obj = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);
			*rval = OBJECT_TO_JSVAL(obj);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_strokeDashOffset(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &value)) return false;
			IMTRY()
				iimpl()->strokeDashOffset(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->strokeDashOffset();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_strokeLineCap(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->strokeLineCap((::Magick::LineCap)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->strokeLineCap();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_strokeLineJoin(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->strokeLineJoin((::Magick::LineJoin)value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->strokeLineJoin();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_strokeMiterLimit(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->strokeMiterLimit(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->strokeMiterLimit();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_strokePattern(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			Image *image = impl::imageFromJsv(argv[0]);
			if(!image)
			{
				JS_ReportError(ecx()->_jsCx, "Image.strokePattern 1st arg must be Image");
				return false;
			}

			IMTRY()
				iimpl()->strokePattern(image->getImpl());
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		Image *image;
		IMTRY()
			image = new Image(iimpl()->strokePattern());
		IMCATCH(return false);

		*rval = image->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_strokeWidth(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &value)) return false;
			IMTRY()
				iimpl()->strokeWidth(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->strokeWidth();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_subImage(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->subImage(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->subImage();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_subRange(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &value)) return false;
			IMTRY()
				iimpl()->subRange(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->subRange();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_tileName(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			char *value;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value)) return false;
			IMTRY()
				iimpl()->tileName(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}
		std::string value;
		IMTRY()
			value = iimpl()->tileName();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_totalColors(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 value;

		IMTRY()
			value = iimpl()->totalColors();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_transformOrigin(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transformOrigin must be called with 2 args");
			return false;
		}
		jsdouble x,y;

		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &x, &y)) return false;
		IMTRY()
			iimpl()->transformOrigin(x, y);
		IMCATCH(return false);
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_transformRotation(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transformOrigin must be called with 1 arg");
			return false;
		}
		jsdouble angle;

		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &angle)) return false;
		IMTRY()
			iimpl()->transformRotation(angle);
		IMCATCH(return false);
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_transformReset(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			iimpl()->transformReset();
		IMCATCH(return false);
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_transformScale(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transformScale must be called with 2 args");
			return false;
		}
		jsdouble sx,sy;

		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &sx, &sy)) return false;
		IMTRY()
			iimpl()->transformScale(sx, sy);
		IMCATCH(return false);
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_transformSkewX(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transformSkewX must be called with 1 arg");
			return false;
		}
		jsdouble value;

		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &value)) return false;
		IMTRY()
			iimpl()->transformSkewX(value);
		IMCATCH(return false);
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_transformSkewY(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transformSkewY must be called with 1 arg");
			return false;
		}
		jsdouble value;

		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &value)) return false;
		IMTRY()
			iimpl()->transformSkewY(value);
		IMCATCH(return false);
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_verbose(uintN argc, jsval *argv, jsval *rval)
	{
		JSBool value;

		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &value)) return false;
			IMTRY()
				iimpl()->verbose(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			value = iimpl()->verbose();
		IMCATCH(return false);

		*rval = BOOLEAN_TO_JSVAL(value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_view(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc)
		{
			char *value;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &value)) return false;
			IMTRY()
				iimpl()->view(value);
			IMCATCH(return false);
			*rval = JSVAL_VOID;
			return true;
		}
		std::string value;
		IMTRY()
			value = iimpl()->view();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, value.data(), value.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_virtualPixelMethod(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 type;
		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &type)) return false;

			IMTRY()
				iimpl()->virtualPixelMethod((::Magick::VirtualPixelMethod)type);
			IMCATCH(return false);
		}

		IMTRY()
			type = iimpl()->virtualPixelMethod();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(type);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////()
	bool ImageAttributes::call_xResolution(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		IMTRY()
			value = iimpl()->xResolution();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::call_yResolution(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble value;

		IMTRY()
			value = iimpl()->yResolution();
		IMCATCH(return false);

		return JS_NewDoubleValue(ecx()->_jsCx, value, rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool ImageAttributes::jsRegister()
	{
		if(!jsRegisterMeth("image", &ImageAttributes::call_image, 0)) return false;
		if(!jsRegisterMeth("adjoin", &ImageAttributes::call_adjoin, 0)) return false;
		if(!jsRegisterMeth("antiAlias", &ImageAttributes::call_antiAlias, 0)) return false;
		if(!jsRegisterMeth("animationDelay", &ImageAttributes::call_animationDelay, 0)) return false;
		if(!jsRegisterMeth("animationIterations", &ImageAttributes::call_animationIterations, 0)) return false;
		if(!jsRegisterMeth("attribute", &ImageAttributes::call_attribute, 0)) return false;
		if(!jsRegisterMeth("backgroundColor", &ImageAttributes::call_backgroundColor, 0)) return false;
		if(!jsRegisterMeth("backgroundTexture", &ImageAttributes::call_backgroundTexture, 0)) return false;
		if(!jsRegisterMeth("baseColumns", &ImageAttributes::call_baseColumns, 0)) return false;
		if(!jsRegisterMeth("baseFilename", &ImageAttributes::call_baseFilename, 0)) return false;
		if(!jsRegisterMeth("baseRows", &ImageAttributes::call_baseRows, 0)) return false;
		if(!jsRegisterMeth("borderColor", &ImageAttributes::call_borderColor, 0)) return false;
		if(!jsRegisterMeth("boundingBox", &ImageAttributes::call_boundingBox, 0)) return false;
		if(!jsRegisterMeth("boxColor", &ImageAttributes::call_boxColor, 0)) return false;
		if(!jsRegisterMeth("chromaBluePrimary", &ImageAttributes::call_chromaBluePrimary, 0)) return false;
		if(!jsRegisterMeth("chromaGreenPrimary", &ImageAttributes::call_chromaGreenPrimary, 0)) return false;
		if(!jsRegisterMeth("chromaRedPrimary", &ImageAttributes::call_chromaRedPrimary, 0)) return false;
		if(!jsRegisterMeth("chromaWhitePoint", &ImageAttributes::call_chromaWhitePoint, 0)) return false;
		if(!jsRegisterMeth("classType", &ImageAttributes::call_classType, 0)) return false;
		if(!jsRegisterMeth("clipMask", &ImageAttributes::call_clipMask, 0)) return false;
		if(!jsRegisterMeth("colorFuzz", &ImageAttributes::call_colorFuzz, 0)) return false;
		if(!jsRegisterMeth("colorMap", &ImageAttributes::call_colorMap, 0)) return false;
		if(!jsRegisterMeth("colorMapSize", &ImageAttributes::call_colorMapSize, 0)) return false;
		if(!jsRegisterMeth("colorSpace", &ImageAttributes::call_colorSpace, 0)) return false;
		if(!jsRegisterMeth("colorspaceType", &ImageAttributes::call_colorspaceType, 0)) return false;
		if(!jsRegisterMeth("columns", &ImageAttributes::call_columns, 0)) return false;
		if(!jsRegisterMeth("compressType", &ImageAttributes::call_compressType, 0)) return false;
		if(!jsRegisterMeth("debug", &ImageAttributes::call_debug, 0)) return false;
		if(!jsRegisterMeth("defineValue", &ImageAttributes::call_defineValue, 0)) return false;
		if(!jsRegisterMeth("defineSet", &ImageAttributes::call_defineSet, 0)) return false;
		if(!jsRegisterMeth("density", &ImageAttributes::call_density, 0)) return false;
		if(!jsRegisterMeth("depth", &ImageAttributes::call_depth, 0)) return false;
		if(!jsRegisterMeth("directory", &ImageAttributes::call_directory, 0)) return false;
		if(!jsRegisterMeth("endian", &ImageAttributes::call_endian, 0)) return false;
		if(!jsRegisterMeth("exifProfile", &ImageAttributes::call_exifProfile, 0)) return false;
		if(!jsRegisterMeth("fileName", &ImageAttributes::call_fileName, 0)) return false;
		if(!jsRegisterMeth("fileSize", &ImageAttributes::call_fileSize, 0)) return false;
		if(!jsRegisterMeth("fillColor", &ImageAttributes::call_fillColor, 0)) return false;
		if(!jsRegisterMeth("fillRule", &ImageAttributes::call_fillRule, 0)) return false;
		if(!jsRegisterMeth("fillPattern", &ImageAttributes::call_fillPattern, 0)) return false;
		if(!jsRegisterMeth("filterType", &ImageAttributes::call_filterType, 0)) return false;
		if(!jsRegisterMeth("font", &ImageAttributes::call_font, 0)) return false;
		if(!jsRegisterMeth("fontPointsize", &ImageAttributes::call_fontPointsize, 0)) return false;
		if(!jsRegisterMeth("fontTypeMetrics", &ImageAttributes::call_fontTypeMetrics, 0)) return false;
		if(!jsRegisterMeth("format", &ImageAttributes::call_format, 0)) return false;
		if(!jsRegisterMeth("geometry", &ImageAttributes::call_geometry, 0)) return false;
		if(!jsRegisterMeth("gifDisposeMethod", &ImageAttributes::call_gifDisposeMethod, 0)) return false;
		if(!jsRegisterMeth("iccColorProfile", &ImageAttributes::call_iccColorProfile, 0)) return false;
		if(!jsRegisterMeth("interlaceType", &ImageAttributes::call_interlaceType, 0)) return false;
		if(!jsRegisterMeth("iptcProfile", &ImageAttributes::call_iptcProfile, 0)) return false;
		if(!jsRegisterMeth("isValid", &ImageAttributes::call_isValid, 0)) return false;
		if(!jsRegisterMeth("lineWidth", &ImageAttributes::call_lineWidth, 0)) return false;
		if(!jsRegisterMeth("magick", &ImageAttributes::call_magick, 0)) return false;
		if(!jsRegisterMeth("matte", &ImageAttributes::call_matte, 0)) return false;
		if(!jsRegisterMeth("matteColor", &ImageAttributes::call_matteColor, 0)) return false;
		if(!jsRegisterMeth("meanErrorPerPixel", &ImageAttributes::call_meanErrorPerPixel, 0)) return false;
		if(!jsRegisterMeth("modulusDepth", &ImageAttributes::call_modulusDepth, 0)) return false;
		if(!jsRegisterMeth("montageGeometry", &ImageAttributes::call_montageGeometry, 0)) return false;
		if(!jsRegisterMeth("monochrome", &ImageAttributes::call_monochrome, 0)) return false;
		if(!jsRegisterMeth("normalizedMaxError", &ImageAttributes::call_normalizedMaxError, 0)) return false;
		if(!jsRegisterMeth("normalizedMeanError", &ImageAttributes::call_normalizedMeanError, 0)) return false;
		if(!jsRegisterMeth("orientation", &ImageAttributes::call_orientation, 0)) return false;
		if(!jsRegisterMeth("page", &ImageAttributes::call_page, 0)) return false;
		if(!jsRegisterMeth("penColor", &ImageAttributes::call_penColor, 0)) return false;
		if(!jsRegisterMeth("penTexture", &ImageAttributes::call_penTexture, 0)) return false;
		if(!jsRegisterMeth("pixelColor", &ImageAttributes::call_pixelColor, 0)) return false;
		if(!jsRegisterMeth("profile", &ImageAttributes::call_profile, 0)) return false;
		if(!jsRegisterMeth("quality", &ImageAttributes::call_quality, 0)) return false;
		if(!jsRegisterMeth("quantizeColors", &ImageAttributes::call_quantizeColors, 0)) return false;
		if(!jsRegisterMeth("quantizeColorSpace", &ImageAttributes::call_quantizeColorSpace, 0)) return false;
		if(!jsRegisterMeth("quantizeDither", &ImageAttributes::call_quantizeDither, 0)) return false;
		if(!jsRegisterMeth("quantizeTreeDepth", &ImageAttributes::call_quantizeTreeDepth, 0)) return false;
		if(!jsRegisterMeth("renderingIntent", &ImageAttributes::call_renderingIntent, 0)) return false;
		if(!jsRegisterMeth("resolutionUnits", &ImageAttributes::call_resolutionUnits, 0)) return false;
		if(!jsRegisterMeth("rows", &ImageAttributes::call_rows, 0)) return false;
		if(!jsRegisterMeth("scene", &ImageAttributes::call_scene, 0)) return false;
		if(!jsRegisterMeth("signature", &ImageAttributes::call_signature, 0)) return false;
		if(!jsRegisterMeth("size", &ImageAttributes::call_size, 0)) return false;
		if(!jsRegisterMeth("strokeAntiAlias", &ImageAttributes::call_strokeAntiAlias, 0)) return false;
		if(!jsRegisterMeth("strokeColor", &ImageAttributes::call_strokeColor, 0)) return false;
		if(!jsRegisterMeth("strokeDashArray", &ImageAttributes::call_strokeDashArray, 0)) return false;
		if(!jsRegisterMeth("strokeDashOffset", &ImageAttributes::call_strokeDashOffset, 0)) return false;
		if(!jsRegisterMeth("strokeLineCap", &ImageAttributes::call_strokeLineCap, 0)) return false;
		if(!jsRegisterMeth("strokeLineJoin", &ImageAttributes::call_strokeLineJoin, 0)) return false;
		if(!jsRegisterMeth("strokeMiterLimit", &ImageAttributes::call_strokeMiterLimit, 0)) return false;
		if(!jsRegisterMeth("strokePattern", &ImageAttributes::call_strokePattern, 0)) return false;
		if(!jsRegisterMeth("strokeWidth", &ImageAttributes::call_strokeWidth, 0)) return false;
		if(!jsRegisterMeth("subImage", &ImageAttributes::call_subImage, 0)) return false;
		if(!jsRegisterMeth("subRange", &ImageAttributes::call_subRange, 0)) return false;
		if(!jsRegisterMeth("tileName", &ImageAttributes::call_tileName, 0)) return false;
		if(!jsRegisterMeth("totalColors", &ImageAttributes::call_totalColors, 0)) return false;
		if(!jsRegisterMeth("transformOrigin", &ImageAttributes::call_transformOrigin, 0)) return false;
		if(!jsRegisterMeth("transformRotation", &ImageAttributes::call_transformRotation, 0)) return false;
		if(!jsRegisterMeth("transformReset", &ImageAttributes::call_transformReset, 0)) return false;
		if(!jsRegisterMeth("transformScale", &ImageAttributes::call_transformScale, 0)) return false;
		if(!jsRegisterMeth("transformSkewX", &ImageAttributes::call_transformSkewX, 0)) return false;
		if(!jsRegisterMeth("transformSkewY", &ImageAttributes::call_transformSkewY, 0)) return false;
		if(!jsRegisterMeth("verbose", &ImageAttributes::call_verbose, 0)) return false;
		if(!jsRegisterMeth("view", &ImageAttributes::call_view, 0)) return false;
		if(!jsRegisterMeth("virtualPixelMethod", &ImageAttributes::call_virtualPixelMethod, 0)) return false;
		if(!jsRegisterMeth("xResolution", &ImageAttributes::call_xResolution, 0)) return false;
		if(!jsRegisterMeth("yResolution", &ImageAttributes::call_yResolution, 0)) return false;

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	::Magick::Image *ImageAttributes::iimpl()
	{
		return &_img->getImpl();
	}




}}
