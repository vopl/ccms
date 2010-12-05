#include "magick_pch.h"
#include "magick/image.hpp"
#include "magick/macroHelpers.hpp"
#include "magick/imageCnvtHelpers.hpp"
#include "magick/drawable.hpp"
#include "magick/imageAttributes.hpp"


namespace ccms{ namespace magick{




























	//////////////////////////////////////////////////////////////////////////
	Image::Image()
		: JsObject(true, "Image")
		, _attributes(NULL)
	{
		jsRegister();
	}


	Image::Image(const ::Magick::Image &from)
		: JsObject(true, "Image")
		, _attributes(NULL)
	{
		IMTRY()
			_impl.reset(new ::Magick::Image(from));
		IMCATCH(return)
		jsRegister();
	}


	bool Image::jsRegister()
	{
		if(!jsRegisterMeth("adaptiveBlur", &Image::call_adaptiveBlur, 0)) return false;
		if(!jsRegisterMeth("adaptiveThreshold", &Image::call_adaptiveThreshold, 0)) return false;
		if(!jsRegisterMeth("addNoise", &Image::call_addNoise, 0)) return false;
		if(!jsRegisterMeth("addNoiseChannel", &Image::call_addNoiseChannel, 0)) return false;
		if(!jsRegisterMeth("affineTransform", &Image::call_affineTransform, 0)) return false;
		if(!jsRegisterMeth("annotate", &Image::call_annotate, 0)) return false;
		if(!jsRegisterMeth("blur", &Image::call_blur, 0)) return false;
		if(!jsRegisterMeth("blurChannel", &Image::call_blurChannel, 0)) return false;
		if(!jsRegisterMeth("border", &Image::call_border, 0)) return false;
		if(!jsRegisterMeth("channel", &Image::call_channel, 0)) return false;
		if(!jsRegisterMeth("channelDepth", &Image::call_channelDepth, 0)) return false;
		if(!jsRegisterMeth("charcoal", &Image::call_charcoal, 0)) return false;
		if(!jsRegisterMeth("chop", &Image::call_chop, 0)) return false;
		if(!jsRegisterMeth("cdl", &Image::call_cdl, 0)) return false;
		if(!jsRegisterMeth("colorize", &Image::call_colorize, 0)) return false;
		if(!jsRegisterMeth("colorMatrix", &Image::call_colorMatrix, 0)) return false;
		if(!jsRegisterMeth("comment", &Image::call_comment, 0)) return false;
		if(!jsRegisterMeth("compose", &Image::call_compose, 0)) return false;
		if(!jsRegisterMeth("compare", &Image::call_compare, 0)) return false;
		if(!jsRegisterMeth("composite", &Image::call_composite, 0)) return false;
		if(!jsRegisterMeth("contrast", &Image::call_contrast, 0)) return false;
		if(!jsRegisterMeth("convolve", &Image::call_convolve, 0)) return false;
		if(!jsRegisterMeth("crop", &Image::call_crop, 0)) return false;
		if(!jsRegisterMeth("cycleColormap", &Image::call_cycleColormap, 0)) return false;
		if(!jsRegisterMeth("despeckle", &Image::call_despeckle, 0)) return false;
		if(!jsRegisterMeth("distort", &Image::call_distort, 0)) return false;
		if(!jsRegisterMeth("draw", &Image::call_draw, 0)) return false;
		if(!jsRegisterMeth("edge", &Image::call_edge, 0)) return false;
		if(!jsRegisterMeth("emboss", &Image::call_emboss, 0)) return false;
		if(!jsRegisterMeth("enhance", &Image::call_enhance, 0)) return false;
		if(!jsRegisterMeth("equalize", &Image::call_equalize, 0)) return false;
		if(!jsRegisterMeth("erase", &Image::call_erase, 0)) return false;
		if(!jsRegisterMeth("extent", &Image::call_extent, 0)) return false;
		if(!jsRegisterMeth("flip", &Image::call_flip, 0)) return false;
		if(!jsRegisterMeth("floodFillColor", &Image::call_floodFillColor, 0)) return false;
		if(!jsRegisterMeth("floodFillOpacity", &Image::call_floodFillOpacity, 0)) return false;
		if(!jsRegisterMeth("floodFillTexture", &Image::call_floodFillTexture, 0)) return false;
		if(!jsRegisterMeth("flop", &Image::call_flop, 0)) return false;
		if(!jsRegisterMeth("frame", &Image::call_frame, 0)) return false;
		if(!jsRegisterMeth("fx", &Image::call_fx, 0)) return false;
		if(!jsRegisterMeth("gamma", &Image::call_gamma, 0)) return false;
		if(!jsRegisterMeth("gaussianBlur", &Image::call_gaussianBlur, 0)) return false;
		if(!jsRegisterMeth("gaussianBlurChannel", &Image::call_gaussianBlurChannel, 0)) return false;
		if(!jsRegisterMeth("getBase64", &Image::call_getBase64, 0)) return false;
		if(!jsRegisterMeth("haldClut", &Image::call_haldClut, 0)) return false;
		if(!jsRegisterMeth("implode", &Image::call_implode, 0)) return false;
		if(!jsRegisterMeth("inverseFourierTransform", &Image::call_inverseFourierTransform, 0)) return false;
		if(!jsRegisterMeth("label", &Image::call_label, 0)) return false;
		if(!jsRegisterMeth("level", &Image::call_level, 0)) return false;
		if(!jsRegisterMeth("levelChannel", &Image::call_levelChannel, 0)) return false;
		if(!jsRegisterMeth("magick", &Image::call_magick, 0)) return false;
		if(!jsRegisterMeth("magnify", &Image::call_magnify, 0)) return false;
		if(!jsRegisterMeth("map", &Image::call_map, 0)) return false;
		if(!jsRegisterMeth("matteFloodfill", &Image::call_matteFloodfill, 0)) return false;
		if(!jsRegisterMeth("medianFilter", &Image::call_medianFilter, 0)) return false;
		if(!jsRegisterMeth("minify", &Image::call_minify, 0)) return false;
		if(!jsRegisterMeth("modulate", &Image::call_modulate, 0)) return false;
		if(!jsRegisterMeth("motionBlur", &Image::call_motionBlur, 0)) return false;
		if(!jsRegisterMeth("negate", &Image::call_negate, 0)) return false;
		if(!jsRegisterMeth("normalize", &Image::call_normalize, 0)) return false;
		if(!jsRegisterMeth("oilPaint", &Image::call_oilPaint, 0)) return false;
		if(!jsRegisterMeth("opacity", &Image::call_opacity, 0)) return false;
		if(!jsRegisterMeth("opaque", &Image::call_opaque, 0)) return false;
		if(!jsRegisterMeth("ping", &Image::call_ping, 0)) return false;
		if(!jsRegisterMeth("quantize", &Image::call_quantize, 0)) return false;
		if(!jsRegisterMeth("quantumOperator", &Image::call_quantumOperator, 0)) return false;
		if(!jsRegisterMeth("process", &Image::call_process, 0)) return false;
		if(!jsRegisterMeth("raise", &Image::call_raise, 0)) return false;
		if(!jsRegisterMeth("randomThreshold", &Image::call_randomThreshold, 0)) return false;
		if(!jsRegisterMeth("randomThresholdChannel", &Image::call_randomThresholdChannel, 0)) return false;
		if(!jsRegisterMeth("read", &Image::call_read, 0)) return false;
		if(!jsRegisterMeth("reduceNoise", &Image::call_reduceNoise, 0)) return false;
		if(!jsRegisterMeth("resize", &Image::call_resize, 0)) return false;
		if(!jsRegisterMeth("roll", &Image::call_roll, 0)) return false;
		if(!jsRegisterMeth("rotate", &Image::call_rotate, 0)) return false;
		if(!jsRegisterMeth("sample", &Image::call_sample, 0)) return false;
		if(!jsRegisterMeth("scale", &Image::call_scale, 0)) return false;
		if(!jsRegisterMeth("segment", &Image::call_segment, 0)) return false;
		if(!jsRegisterMeth("shade", &Image::call_shade, 0)) return false;
		if(!jsRegisterMeth("sharpen", &Image::call_sharpen, 0)) return false;
		if(!jsRegisterMeth("sharpenChannel", &Image::call_sharpenChannel, 0)) return false;
		if(!jsRegisterMeth("shave", &Image::call_shave, 0)) return false;
		if(!jsRegisterMeth("shear", &Image::call_shear, 0)) return false;
		if(!jsRegisterMeth("sigmoidalContrast", &Image::call_sigmoidalContrast, 0)) return false;
		if(!jsRegisterMeth("solarize", &Image::call_solarize, 0)) return false;
		if(!jsRegisterMeth("splice", &Image::call_splice, 0)) return false;
		if(!jsRegisterMeth("spread", &Image::call_spread, 0)) return false;
		if(!jsRegisterMeth("sparseColor", &Image::call_sparseColor, 0)) return false;
		if(!jsRegisterMeth("stegano", &Image::call_stegano, 0)) return false;
		if(!jsRegisterMeth("stereo", &Image::call_stereo, 0)) return false;
		if(!jsRegisterMeth("swirl", &Image::call_swirl, 0)) return false;
		if(!jsRegisterMeth("texture", &Image::call_texture, 0)) return false;
		if(!jsRegisterMeth("threshold", &Image::call_threshold, 0)) return false;
		if(!jsRegisterMeth("transform", &Image::call_transform, 0)) return false;
		if(!jsRegisterMeth("transparent", &Image::call_transparent, 0)) return false;
		if(!jsRegisterMeth("transparentChroma", &Image::call_transparentChroma, 0)) return false;
		if(!jsRegisterMeth("trim", &Image::call_trim, 0)) return false;
		if(!jsRegisterMeth("type", &Image::call_type, 0)) return false;
		if(!jsRegisterMeth("unsharpmask", &Image::call_unsharpmask, 0)) return false;
		if(!jsRegisterMeth("unsharpmaskChannel", &Image::call_unsharpmaskChannel, 0)) return false;
		if(!jsRegisterMeth("wave", &Image::call_wave, 0)) return false;
		if(!jsRegisterMeth("write", &Image::call_write, 0)) return false;
		if(!jsRegisterMeth("zoom", &Image::call_zoom, 0)) return false;

		if(!jsRegisterMeth("statistics", &Image::call_statistics, 0)) return false;
		if(!jsRegisterMeth("attributes", &Image::call_attributes, 0)) return false;


		if(!jsRegisterMeth("modifyImage", &Image::call_modifyImage, 0)) return false;
		if(!jsRegisterMeth("registerId", &Image::call_registerId, 0)) return false;
		if(!jsRegisterMeth("unregisterId", &Image::call_unregisterId, 0)) return false;

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	bool Image::construct(uintN argc, jsval *argv)
	{
		_imgStub = NULL;

		//конструкторы всякие
		bool constructed = false;
		if(!argc)
		{//без аргументов

			IMTRY()
				_impl.reset(new Magick::Image);
			IMCATCH(return false)

			constructed = true;
		}
		else if(1 == argc)
		{//1 аргумент - либо объект Image либо строка spec
			if(JSVAL_IS_NULL(argv[0]))
			{
				JS_ReportError(ecx()->_jsCx, "Image constructor: bad params");
				return false;
			}
			if(JSVAL_IS_OBJECT(argv[0]))
			{//объект попробывать как Image
				Image *ijso = impl::imageFromJsv(argv[0]);
				if(ijso)
				{
					IMTRY()
						_impl.reset(new Magick::Image(ijso->getImpl()));
					IMCATCH(return false)
					constructed = true;
				}
			}

			if(!constructed)
			{//объект не прошел, трактовать как строку spec
				char *imageSpec_;
				JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &imageSpec_);

				IMTRY()
					_impl.reset(new Magick::Image(imageSpec_));
				IMCATCH(return false)
				constructed = true;
			}

		}
		else if(2 == argc)
		{//Geometry *size, Color *color

			boost::shared_ptr< ::Magick::Geometry> geometry;
			if(!impl::geometryFromJsv(argv[0], geometry)) return false;
			if(!geometry)
			{
				JS_ReportError(ecx()->_jsCx, "Image constructor: 1st arg must be a Geometry");
				return false;
			}

			boost::shared_ptr< ::Magick::Color> color;
			if(!impl::colorFromJsv(argv[1], color)) return false;
			if(!color)
			{
				JS_ReportError(ecx()->_jsCx, "Image constructor: 2nd arg must be a Color");
				return false;
			}

			IMTRY()
				_impl.reset(new Magick::Image(*geometry, *color));
			IMCATCH(return false)
			constructed = true;
		}
		else
		{//не 1 и не 2 аргумента
			JS_ReportError(ecx()->_jsCx, "Image constructor: bad params");
			return false;
		}

		if(constructed)
		{
			_imgStub = _impl.get();
			return true;
		}

		assert(0);
		return false;
	}






	//////////////////////////////////////////////////////////////////////////
	Image::~Image()
	{

	}














	//////////////////////////////////////////////////////////////////////////
	bool Image::call_adaptiveBlur(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble radius = 0.0;
		jsdouble sigma = 1.0;

		if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &radius, &sigma)) return false;
		}
		else if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &radius)) return false;
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->adaptiveBlur(radius, sigma);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_adaptiveThreshold(uintN argc, jsval *argv, jsval *rval)
	{
		if(3 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image::adaptiveThreshold must be called with 3 args");
			return false;
		}

		uint32 width, height;
		int32 offset;
		if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "uui", &width, &height, &offset)) return false;

		IMTRY()
			_imgStub->adaptiveThreshold(width, height, offset);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_addNoise(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image::addNoise must be called with 1 arg");
			return false;
		}

		int32 type;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "i", &type))return false;

		IMTRY()
			_imgStub->addNoise((::Magick::NoiseType)type);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Image::call_addNoiseChannel(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image::addNoiseChannel must be called with 2 args");
			return false;
		}

		uint32 ctype, ntype;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "uu", &ctype, &ntype))return false;

		IMTRY()
			_imgStub->addNoiseChannel((::Magick::ChannelType)ctype, (::Magick::NoiseType)ntype);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_affineTransform(uintN argc, jsval *argv, jsval *rval)
	{
		if(6 == argc)
		{
			jsdouble sx_, sy_, rx_, ry_, tx_, ty_;
			if(!JS_ConvertArguments(ecx()->_jsCx, 6, argv, "dddddd", &sx_, &sy_, &rx_, &ry_, &tx_, &ty_))return false;

			IMTRY()
				::Magick::DrawableAffine da(sx_, sy_, rx_, ry_, tx_, ty_);
				_imgStub->affineTransform(da);
			IMCATCH(return false);
		}
		else if(1 == argc)
		{
			JSObject *jso;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "o", &jso))return false;

			DrawableAffine *affine = dynamic_cast<DrawableAffine *>(thisObj(jso));
			if(!affine)
			{
				JS_ReportError(ecx()->_jsCx, "Image::affineTransform must be called DrawableAffine object as arg");
				return false;
			}

			IMTRY()
				_imgStub->affineTransform(affine->getImpl());
			IMCATCH(return false);
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "Image::affineTransform must be called with 1 or 6 args");
			return false;
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_annotate(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 > argc || 4 < argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image::annotate must be called with 2-4 args");
			return false;
		}

		char *text;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &text)) return false;

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[1], geometry)) return false;


		if(2 == argc)
		{
			// ( const std::string &text_, const Geometry &location_ );
			// ( const std::string &text_, const GravityType gravity_ );

			if(geometry)
			{
				IMTRY()
					_imgStub->annotate(text, *geometry);
				IMCATCH(return false);
			}
			else if(JSVAL_IS_NUMBER(argv[1]))
			{
				uint32 gravityType;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "u", &gravityType))return false;
				IMTRY()
					_imgStub->annotate(text, (::Magick::GravityType)gravityType);
				IMCATCH(return false);
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "Image::annotate 2nd arg must be a Geometry object, valid Geometry string or GravityType value");
				return false;
			}
		}
		else if(3 == argc)
		{
			// ( const std::string &text_, const Geometry &boundingArea_, const GravityType gravity_ );
			if(!geometry)
			{
				JS_ReportError(ecx()->_jsCx, "Image::annotate 2nd arg must be a Geometry object, valid Geometry string");
				return false;
			}

			uint32 gravityType;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+2, "u", &gravityType))return false;
			IMTRY()
				_imgStub->annotate(text, *geometry, (::Magick::GravityType)gravityType);
			IMCATCH(return false);
		}
		else
		{
			// ( const std::string &text_, const Geometry &boundingArea_, const GravityType gravity_, const double degrees_ );
			if(!geometry)
			{
				JS_ReportError(ecx()->_jsCx, "Image::annotate 2nd arg must be a Geometry object, valid Geometry string");
				return false;
			}

			uint32 gravityType;
			jsdouble degress;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv+2, "ud", &gravityType, &degress))return false;
			IMTRY()
				_imgStub->annotate(text, *geometry, (::Magick::GravityType)gravityType, degress);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_blur(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble radius = 0.0;
		jsdouble sigma = 1.0;

		if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &radius, &sigma))
			{
				return false;
			}
		}
		else if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &radius))
			{
				return false;
			}
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->blur(radius, sigma);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_blurChannel(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "Image::blurChannel must be called with 1-3 args");
			return false;
		}

		uint32 channel;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &channel))
		{
			return false;
		}
		argc--;
		argv++;

		jsdouble radius = 0.0;
		jsdouble sigma = 1.0;

		if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &radius, &sigma))
			{
				return false;
			}
		}
		else if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &radius))
			{
				return false;
			}
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->blurChannel((::Magick::ChannelType)channel, radius, sigma);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Image::call_border(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc>0)
		{
			boost::shared_ptr< ::Magick::Geometry> geometry;
			if(!impl::geometryFromJsv(argv[0], geometry)) return false;

			if(!geometry)
			{
				JS_ReportError(ecx()->_jsCx, "Image::border 1st arg must be an Geometry");
				return false;
			}

			IMTRY()
				_imgStub->border(*geometry);
			IMCATCH(return false);
		}
		else
		{
			IMTRY()
				_imgStub->border();
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_channel(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "Image::channel must be called with 1 arg");
			return false;
		}

		uint32 channel;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &channel))
		{
			return false;
		}

		IMTRY()
			_imgStub->channel((::Magick::ChannelType)channel);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_channelDepth(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "Image::channel must be called with 1 arg");
			return false;
		}

		uint32 channel;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &channel))
		{
			return false;
		}

		if(1 < argc)
		{
			uint32 depth;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "u", &depth))
			{
				return false;
			}
			IMTRY()
				_imgStub->channelDepth((::Magick::ChannelType)channel, depth);
				depth = _imgStub->channelDepth((::Magick::ChannelType)channel);
				*rval = INT_TO_JSVAL((int)depth);
			IMCATCH(return false);
		}
		else
		{
			IMTRY()
				uint32 depth = _imgStub->channelDepth((::Magick::ChannelType)channel);
				*rval = INT_TO_JSVAL((int)depth);
			IMCATCH(return false);
		}



		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_charcoal(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble radius = 0.0;
		jsdouble sigma = 1.0;

		if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &radius, &sigma))
			{
				return false;
			}
		}
		else if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &radius))
			{
				return false;
			}
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->charcoal(radius, sigma);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_chop(uintN argc, jsval *argv, jsval *rval)
	{
		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;

		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image::chop 1st arg must be an Geometry");
			return false;
		}

		IMTRY()
			_imgStub->chop(*geometry);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_cdl(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image::cdl must be called with 1 arg");
			return false;
		}

		char *cdl;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &cdl)) return false;

		IMTRY()
			_imgStub->cdl(cdl);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_colorize(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 == argc)
		{
			//opacity, color

			uint32 opacity;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &opacity)) return false;

			boost::shared_ptr< ::Magick::Color> color;
			if(!impl::colorFromJsv(argv[1], color)) return false;
			if(!color)
			{
				JS_ReportError(ecx()->_jsCx, "Image.colorize 2nd arg must be a Color value");
				return false;
			}

			IMTRY()
				_imgStub->colorize(opacity, *color);
			IMCATCH(return false);
		}
		else if(4 == argc)
		{
			//ropacity, gopacity, bopacity, color

			uint32 ropacity, gopacity, bopacity;
			if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "uuu", &ropacity, &gopacity, &bopacity)) return false;

			boost::shared_ptr< ::Magick::Color> color;
			if(!impl::colorFromJsv(argv[3], color)) return false;
			if(!color)
			{
				JS_ReportError(ecx()->_jsCx, "Image.colorize 4nd arg must be a Color value");
				return false;
			}

			IMTRY()
				_imgStub->colorize(ropacity, gopacity, bopacity, *color);
			IMCATCH(return false);
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "Image::colorize must be called with 2 or 4 args");
			return false;
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_colorMatrix(uintN argc, jsval *argv, jsval *rval)
	{
		JSContext *cx = ecx()->_jsCx;

		std::vector<jsval> jsvHolder4Array;
		if(1 == argc && JSVAL_IS_OBJECT(argv[0]))
		{
			JSObject *jso = JSVAL_TO_OBJECT(argv[0]);
			if(JS_IsArrayObject(cx, jso))
			{
				jsuint len;
				if(!JS_GetArrayLength(cx, jso, &len)) return false;

				jsvHolder4Array.resize(len);
				for(jsuint i(0); i<len; i++)
				{
					if(!JS_GetElement(cx, jso, i, &jsvHolder4Array[i])) return false;
				}

				argc = len;
				if(argc)
				{
					argv = &jsvHolder4Array[0];
				}
			}
			else
			{
				JS_ReportError(cx, "Image.colorMatrix first arg must be a Array");
				return false;
			}
		}


		std::vector<jsdouble> matrix;
		size_t order;
		if(1 == argc)
		{
			order = 1;
			matrix.resize(1);
			if(!JS_ConvertArguments(cx, 1, argv, "d", &matrix[0])) return false;
		}
		else if(4 == argc)
		{
			order = 2;
			matrix.resize(4);
			if(!JS_ConvertArguments(cx, 4, argv, "dddd", &matrix[0], &matrix[1], &matrix[2], &matrix[3])) return false;
		}
		else if(9 == argc)
		{
			order = 3;
			matrix.resize(9);
			if(!JS_ConvertArguments(cx, 9, argv, "ddddddddd", 
				&matrix[0], &matrix[1], &matrix[2], 
				&matrix[3], &matrix[4], &matrix[5], 
				&matrix[6], &matrix[7], &matrix[8])) return false;
		}
		else if(16 == argc)
		{
			order = 4;
			matrix.resize(16);
			if(!JS_ConvertArguments(cx, 16, argv, "dddddddddddddddd", 
				&matrix[0], &matrix[1], &matrix[2], &matrix[3],
				&matrix[4], &matrix[5], &matrix[6], &matrix[7],
				&matrix[8], &matrix[9], &matrix[10],&matrix[11],
				&matrix[12],&matrix[13],&matrix[14],&matrix[15])) return false;
		}
		else if(25 == argc)
		{
			order = 5;
			matrix.resize(25);
			if(!JS_ConvertArguments(cx, 25, argv, "ddddddddddddddddddddddddd", 
				&matrix[0], &matrix[1], &matrix[2], &matrix[3], &matrix[4],
				&matrix[5], &matrix[6], &matrix[7], &matrix[8], &matrix[9],
				&matrix[10],&matrix[11],&matrix[12],&matrix[13],&matrix[14],
				&matrix[15],&matrix[16],&matrix[17],&matrix[18],&matrix[19],
				&matrix[20],&matrix[21],&matrix[22],&matrix[23],&matrix[24])) return false;
		}
		else
		{
			JS_ReportError(cx, "Image.colorMatrix must be called with 1,2,4,9,16 or 25 args");
			return false;
		}

		IMTRY()
			_imgStub->colorMatrix(order, &matrix[0]);
		IMCATCH(return false);
		
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_comment(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc>0)
		{
			char *str;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &str)) return false;
			IMTRY()
				_imgStub->comment(str);
			IMCATCH(return false);
		}
		else
		{
			IMTRY()
				_imgStub->comment();
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_compose(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc>0)
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &v)) return false;
			IMTRY()
				_imgStub->compose((::Magick::CompositeOperator)v);
			IMCATCH(return false);
		}
		else
		{
			IMTRY()
				_imgStub->compose();
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_compare(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 1)
		{
			JS_ReportError(ecx()->_jsCx, "Image.compare must be called with 1 arg");
			return false;
		}

		Image *obj = impl::imageFromJsv(argv[0]);

		if(!obj)
		{
			JS_ReportError(ecx()->_jsCx, "Image.compare 1st arg must be a Image object");
			return false;
		}

		IMTRY()
			*rval = BOOLEAN_TO_JSVAL(_imgStub->compare(obj->getImpl()));
		IMCATCH(return false);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_composite(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc && 3 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.composite must be called with 2 or 3 args");
			return false;
		}

		Image *image = impl::imageFromJsv(argv[0]);
		if(!image)
		{
			JS_ReportError(ecx()->_jsCx, "Image.composite 1st arg must be a Image object");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[1], geometry)) return false;

		uint32 gravity;
		if(!geometry)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "u", &gravity)) return false;
		}

		if(2 == argc)
		{
// 			const Image &compositeImage_, const Geometry &offset_
// 			const Image &compositeImage_, const GravityType gravity_
			if(geometry)
			{
				IMTRY()
					_imgStub->composite(image->getImpl(), *geometry);
				IMCATCH(return false);
			}
			else
			{
				IMTRY()
					_imgStub->composite(image->getImpl(), (::Magick::GravityType)gravity);
				IMCATCH(return false);
			}
		}
		else if(3 == argc)
		{
// 			const Image &compositeImage_, const Geometry &offset_,	const CompositeOperator compose_
// 			const Image &compositeImage_, const GravityType gravity_, const CompositeOperator compose_
			uint32 compose;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+2, "u", &compose)) return false;

			if(geometry)
			{
				IMTRY()
					_imgStub->composite(image->getImpl(), *geometry, (::Magick::CompositeOperator)compose);
				IMCATCH(return false);
			}
			else
			{
				IMTRY()
					_imgStub->composite(image->getImpl(), (::Magick::GravityType)gravity, (::Magick::CompositeOperator)compose);
				IMCATCH(return false);
			}
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_contrast(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.contrast must be called with 1 arg");
			return false;
		}

		uint32 sharpen;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &sharpen)) return false;

		IMTRY()
			_imgStub->contrast(sharpen);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_convolve(uintN argc, jsval *argv, jsval *rval)
	{
		JSContext *cx = ecx()->_jsCx;

		std::vector<jsval> jsvHolder4Array;
		if(1 == argc && JSVAL_IS_OBJECT(argv[0]))
		{
			JSObject *jso = JSVAL_TO_OBJECT(argv[0]);
			if(JS_IsArrayObject(cx, jso))
			{
				jsuint len;
				if(!JS_GetArrayLength(cx, jso, &len)) return false;

				jsvHolder4Array.resize(len);
				for(jsuint i(0); i<len; i++)
				{
					if(!JS_GetElement(cx, jso, i, &jsvHolder4Array[i])) return false;
				}

				argc = len;
				if(argc)
				{
					argv = &jsvHolder4Array[0];
				}
			}
			else
			{
				JS_ReportError(cx, "Image.convolve first arg must be a Array");
				return false;
			}
		}

		if(!argc)
		{
			return true;
		}


		std::vector<jsdouble> matrix(argc);
		size_t order=0;

		for(size_t i(0); i<argc; i++)
		{
			if(!JS_ConvertArguments(cx, 1, argv+i, "d", &matrix[i])) return false;
		}
		while(order*order <= argc) order++;
		order--;

		IMTRY()
			_imgStub->convolve(order, &matrix[0]);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_crop(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.crop must be called with 1 arg");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.crop 1st arg must be a Geometry value");
			return false;
		}

		IMTRY()
			_imgStub->crop(*geometry);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_cycleColormap(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.cycleColormap must be called with 1 arg");
			return false;
		}

		uint32 amount;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &amount)) return false;

		IMTRY()
			_imgStub->cycleColormap(amount);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_despeckle(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->despeckle();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_distort(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc && 3 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.distort must be called with 2 or 3 args");
			return false;
		}

		uint32 method;
		JSObject *jso;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "uo", &method, &jso)) return false;

		if(!JS_IsArrayObject(ecx()->_jsCx, jso))
		{
			JS_ReportError(ecx()->_jsCx, "Image.distort 2nd arg must be a Array of numbers");
			return false;
		}

		jsuint pointAmount;
		if(!JS_GetArrayLength(ecx()->_jsCx, jso, &pointAmount)) return false;
		std::vector<jsdouble> points(pointAmount);
		for(size_t i(0); i<pointAmount; i++)
		{
			jsval jsv;
			if(!JS_GetElement(ecx()->_jsCx, jso, i, &jsv)) return false;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "d", &points[i])) return false;
		}

		JSBool bestfit = false;
		if(3 == argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+2, "b", &bestfit)) return false;
		}

		IMTRY()
			if(points.size())
			{
				_imgStub->distort((::Magick::DistortImageMethod)method, points.size(), &points[0], bestfit);
			}
			else
			{
				_imgStub->distort((::Magick::DistortImageMethod)method, 0, NULL, bestfit);
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_draw(uintN argc, jsval *argv, jsval *rval)
	{
		::Magick::DrawableList list;
		for(uintN i(0); i<argc; i++)
		{
			if(!impl::DrawableList_jsv(argv[i], list)) return false;
		}
		if(list.empty())
		{
			*rval = JSVAL_VOID;
			return true;
		}

		IMTRY()
			_imgStub->draw(list);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_edge(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble radius = 0.0;
		if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &radius))
			{
				return false;
			}
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->edge(radius);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_emboss(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble radius = 0.0;
		jsdouble sigma = 1.0;

		if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &radius, &sigma))
			{
				return false;
			}
		}
		else if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &radius))
			{
				return false;
			}
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->emboss(radius, sigma);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_enhance(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->enhance();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_equalize(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->equalize();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_erase(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->erase();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_extent(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "Image.extent must be called with 1 arg");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.extent 1st arg must be called a Geometry object or string");
			return false;
		}

		boost::shared_ptr< ::Magick::Color> color;
		uint32 gravityType = (uint32)-1;
		if(argc >= 2)
		{
			if(JSVAL_IS_INT(argv[1]))
			{
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "u", &gravityType))return false;
			}
			else
			{
				if(!impl::colorFromJsv(argv[1], color)) return false;
			}
		}

		if(argc >= 3)
		{
			if(JSVAL_IS_INT(argv[2]))
			{
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+2, "u", &gravityType))return false;
			}
			else
			{
				if(!impl::colorFromJsv(argv[2], color)) return false;
			}
		}

		IMTRY()
			if(color && gravityType!=-1)
			{
				_imgStub->extent(*geometry, *color, (::Magick::GravityType)gravityType);
			}
			else if(color)
			{
				_imgStub->extent(*geometry, *color);
			}
			else if(gravityType!=-1)
			{
				_imgStub->extent(*geometry, (::Magick::GravityType)gravityType);
			}
			else
			{
				_imgStub->extent(*geometry);
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_flip(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->flip();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_floodFillColor(uintN argc, jsval *argv, jsval *rval)
	{
		//geometry X (x,y)
		//color X (color,color)
		if(argc < 2 || argc > 4)
		{
			JS_ReportError(ecx()->_jsCx, "Image.floodFillColor must be called with 2-4 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		int32 x,y;

		if(geometry)
		{
			argc--;
			argv++;
		}
		else
		{
			if(!JSVAL_IS_INT(argv[0]) || !JSVAL_IS_INT(argv[1]))
			{
				JS_ReportError(ecx()->_jsCx, "Image.floodFillColor 1st and 2nd args must be a integer");
				return false;
			}
			x = JSVAL_TO_INT(argv[0]);
			argc--;
			argv++;

			y = JSVAL_TO_INT(argv[0]);
			argc--;
			argv++;
		}

		if(!argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.floodFillColor color must be passed");
			return false;
		}

		boost::shared_ptr< ::Magick::Color> fillColor;
		if(!impl::colorFromJsv(argv[0], fillColor)) return false;
		if(!fillColor)
		{
			JS_ReportError(ecx()->_jsCx, "Image.floodFillColor wrong fillColor value");
			return false;
		}
		argc--;
		argv++;

		boost::shared_ptr< ::Magick::Color> bgColor;
		if(argc)
		{
			if(!impl::colorFromJsv(argv[0], bgColor)) return false;
			if(!bgColor)
			{
				JS_ReportError(ecx()->_jsCx, "Image.floodFillColor wrong bgColor value");
				return false;
			}
			argc--;
			argv++;
		}

		IMTRY()

			if(geometry)
			{
				if(bgColor)
				{
					_imgStub->floodFillColor(*geometry, *fillColor, *bgColor);
				}
				else
				{
					_imgStub->floodFillColor(*geometry, *fillColor);
				}
			}
			else
			{
				if(bgColor)
				{
					_imgStub->floodFillColor(x, y, *fillColor, *bgColor);
				}
				else
				{
					_imgStub->floodFillColor(x, y, *fillColor);
				}
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_floodFillOpacity(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 4)
		{
			JS_ReportError(ecx()->_jsCx, "Image.floodFillOpacity must be called with 4 args");
			return false;
		}
		int32 x,y;
		uint32 opacity, method;
		if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "iiuu", &x, &y, &opacity, &method)) return false;

		IMTRY()
			_imgStub->floodFillOpacity(x, y, opacity, (::Magick::PaintMethod)method);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_floodFillTexture(uintN argc, jsval *argv, jsval *rval)
	{
		//geometry X (x,y)
		//image X (image,color)
		if(argc < 2 || argc > 4)
		{
			JS_ReportError(ecx()->_jsCx, "Image.floodFillTexture must be called with 2-4 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		int32 x,y;

		if(geometry)
		{
			argc--;
			argv++;
		}
		else
		{
			if(!JSVAL_IS_INT(argv[0]) || !JSVAL_IS_INT(argv[1]))
			{
				JS_ReportError(ecx()->_jsCx, "Image.floodFillTexture 1st and 2nd args must be a integer");
				return false;
			}
			x = JSVAL_TO_INT(argv[0]);
			argc--;
			argv++;

			y = JSVAL_TO_INT(argv[0]);
			argc--;
			argv++;
		}

		if(!argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.floodFillTexture texture must be passed");
			return false;
		}

		Image *texture = impl::imageFromJsv(argv[0]);
		if(!texture)
		{
			JS_ReportError(ecx()->_jsCx, "Image.floodFillTexture wrong texture value");
			return false;
		}
		argc--;
		argv++;

		boost::shared_ptr< ::Magick::Color> bgColor;
		if(argc)
		{
			if(!impl::colorFromJsv(argv[0], bgColor)) return false;
			if(!bgColor)
			{
				JS_ReportError(ecx()->_jsCx, "Image.floodFillTexture wrong bgColor value");
				return false;
			}
			argc--;
			argv++;
		}

		IMTRY()

			if(geometry)
			{
				if(bgColor)
				{
					_imgStub->floodFillTexture(*geometry, texture->getImpl(), *bgColor);
				}
				else
				{
					_imgStub->floodFillTexture(*geometry, texture->getImpl());
				}
			}
			else
			{
				if(bgColor)
				{
					_imgStub->floodFillTexture(x, y, texture->getImpl(), *bgColor);
				}
				else
				{
					_imgStub->floodFillTexture(x, y, texture->getImpl());
				}
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_flop(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->flop();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_frame(uintN argc, jsval *argv, jsval *rval)
	{
		boost::shared_ptr< ::Magick::Geometry> geometry;
		int32 x,y;
		if(argc)
		{
			if(!impl::geometryFromJsv(argv[0], geometry)) return false;

			if(!geometry)
			{
				if(argc<2)
				{
					JS_ReportError(ecx()->_jsCx, "Image.frame need 2 coordinates or Geometry");
					return false;
				}
				if(!JSVAL_IS_INT(argv[0]) || !JSVAL_IS_INT(argv[1]))
				{
					JS_ReportError(ecx()->_jsCx, "Image.frame 1st and 2nd args must be a integer");
					return false;
				}
				x = JSVAL_TO_INT(argv[0]);
				argc--;
				argv++;

				y = JSVAL_TO_INT(argv[0]);
				argc--;
				argv++;
			}
			else
			{
				argc--;
				argv++;
			}
		}

		if(geometry && argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.frame Geometry accepted, no more args need");
			return false;
		}

		int32 innerBevel=6;
		int32 outerBevel=6;

		if(!geometry)
		{
			if(argc)
			{
				if(!JSVAL_IS_INT(argv[0]))
				{
					JS_ReportError(ecx()->_jsCx, "Image.frame innerBevel must be a integer");
					return false;
				}
				innerBevel = JSVAL_TO_INT(argv[0]);
				argc--;
				argv++;
			}
			if(argc)
			{
				if(!JSVAL_IS_INT(argv[0]))
				{
					JS_ReportError(ecx()->_jsCx, "Image.frame outerBevel must be a integer");
					return false;
				}
				outerBevel = JSVAL_TO_INT(argv[0]);
				argc--;
				argv++;
			}
		}
		IMTRY()
			if(geometry)
			{
				_imgStub->frame(*geometry);
			}
			else
			{
				_imgStub->frame(x, y, innerBevel, outerBevel);
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_fx(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc<1 || argc>2)
		{
			JS_ReportError(ecx()->_jsCx, "Image.fx must be called with 1-2 args");
			return false;
		}
		char *expression;
		uint32 channelType = (uint32)-1;
		if(1 == argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &expression)) return false;
		}
		else
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "su", &expression, &channelType)) return false;
		}
		IMTRY()
			if(channelType == -1)
			{
				_imgStub->fx(expression);
			}
			else
			{
				_imgStub->fx(expression, (::Magick::ChannelType)channelType);
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_gamma(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=0 && argc!=1 && argc!=3)
		{
			JS_ReportError(ecx()->_jsCx, "Image.gamma must be called with 0, 1 or 3 args");
			return false;
		}

		if(1 == argc)
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &v)) return false;
			IMTRY()
				_imgStub->gamma(v);
			IMCATCH(return false);
		}
		if(3 == argc)
		{
			jsdouble vr,vg,vb;
			if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "ddd", &vr,&vg,&vb)) return false;
			IMTRY()
				_imgStub->gamma(vr,vg,vb);
			IMCATCH(return false);
		}

		return JS_NewDoubleValue(ecx()->_jsCx, _imgStub->gamma(), rval);
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_gaussianBlur(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=2)
		{
			JS_ReportError(ecx()->_jsCx, "Image.gaussianBlur must be called with 2 args");
			return false;
		}

		jsdouble width;
		jsdouble sigma;

		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &width, &sigma))return false;
		
		IMTRY()
			_imgStub->gaussianBlur(width, sigma);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_gaussianBlurChannel(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=3)
		{
			JS_ReportError(ecx()->_jsCx, "Image.gaussianBlurChannel must be called with 3 args");
			return false;
		}

		jsdouble width;
		jsdouble sigma;
		uint32 channel;

		if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "udd", &channel, &width, &sigma))return false;

		IMTRY()
			_imgStub->gaussianBlurChannel((::Magick::ChannelType)channel, width, sigma);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_getBase64(uintN argc, jsval *argv, jsval *rval)
	{
		::Magick::Blob blob;
		IMTRY()
			_imgStub->write(&blob);
		IMCATCH(return false);

		std::string base64 = blob.base64();
		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, base64.data(), base64.size()));
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Image::call_haldClut(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=1)
		{
			JS_ReportError(ecx()->_jsCx, "Image.haldClut must be called with 1 arg");
			return false;
		}

		Image *image = impl::imageFromJsv(argv[0]);
		if(!image)
		{
			JS_ReportError(ecx()->_jsCx, "Image.haldClut 1st arg must be Image object");
			return false;
		}

		IMTRY()
			_imgStub->haldClut(image->getImpl());
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_implode(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=1)
		{
			JS_ReportError(ecx()->_jsCx, "Image.implode must be called with 1 arg");
			return false;
		}

		jsdouble factor;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &factor)) return false;

		IMTRY()
			_imgStub->implode(factor);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	bool Image::call_inverseFourierTransform(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=1 && argc!=2)
		{
			JS_ReportError(ecx()->_jsCx, "Image.inverseFourierTransform must be called with 1 or 2 args");
			return false;
		}

		Image *image = impl::imageFromJsv(argv[0]);
		if(!image)
		{
			JS_ReportError(ecx()->_jsCx, "Image.inverseFourierTransform 1st arg must be Image object");
			return false;
		}

		if(argc==1)
		{
			IMTRY()
				_imgStub->inverseFourierTransform(image->getImpl());
			IMCATCH(return false);
		}
		else
		{
			JSBool magnitude;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "b", &magnitude)) return false;

			IMTRY()
				_imgStub->inverseFourierTransform(image->getImpl(), magnitude);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_label(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=0 && 1!=argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.label must be called with 0 or 1 arg");
			return false;
		}

		if(argc)
		{
			char *label;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &label)) return false;
			IMTRY()
				_imgStub->label(label);
			IMCATCH(return false);
		}

		std::string label;
		IMTRY()
			label = _imgStub->label();
		IMCATCH(return false);

		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, label.data(), label.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_level(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=2 && argc!=3)
		{
			JS_ReportError(ecx()->_jsCx, "Image.level must be called with 2 or 3 args");
			return false;
		}

		jsdouble black_point;
		jsdouble white_point;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &black_point, &white_point)) return false;

		if(argc>2)
		{
			jsdouble mid_point;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+2, "d", &mid_point)) return false;
			IMTRY()
				_imgStub->level(black_point, white_point, mid_point);
			IMCATCH(return false);
		}
		else
		{
			IMTRY()
				_imgStub->level(black_point, white_point);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	bool Image::call_magick(uintN argc, jsval *argv, jsval *rval)
	{
		if(!argc)
		{
			IMTRY()
				std::string m = _imgStub->magick();
				*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, m.data(), m.size()));
				return true;
			IMCATCH(return false);
		}
		if(argc!=1)
		{
			JS_ReportError(ecx()->_jsCx, "Image.magick must be called with 1 arg");
			return false;
		}
		char *format;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &format)) return false;

		IMTRY()
			_imgStub->magick(format);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_levelChannel(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=3 && argc!=4)
		{
			JS_ReportError(ecx()->_jsCx, "Image.levelChannel must be called with 3 or 4 args");
			return false;
		}

		uint32 channel;
		jsdouble black_point;
		jsdouble white_point;
		if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "udd", &channel, &black_point, &white_point)) return false;

		if(argc>3)
		{
			jsdouble mid_point;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+3, "d", &mid_point)) return false;
			IMTRY()
				_imgStub->levelChannel((::Magick::ChannelType)channel, black_point, white_point, mid_point);
			IMCATCH(return false);
		}
		else
		{
			IMTRY()
				_imgStub->levelChannel((::Magick::ChannelType)channel, black_point, white_point);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_magnify(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->magnify();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_map(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc!=1 && argc!=2)
		{
			JS_ReportError(ecx()->_jsCx, "Image.map must be called with 1 or 2 args");
			return false;
		}

		Image *image = impl::imageFromJsv(argv[0]);
		if(!image)
		{
			JS_ReportError(ecx()->_jsCx, "Image.map 1st arg must be Image object");
			return false;
		}

		if(argc==1)
		{
			IMTRY()
				_imgStub->map(image->getImpl());
			IMCATCH(return false);
		}
		else
		{
			JSBool dither;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "b", &dither)) return false;

			IMTRY()
				_imgStub->map(image->getImpl(), dither);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_matteFloodfill(uintN argc, jsval *argv, jsval *rval)
	{
		if(5 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.matteFloodfill must be called with 5 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Color> target;
		if(!impl::colorFromJsv(argv[0], target)) return false;
		if(!target)
		{
			JS_ReportError(ecx()->_jsCx, "Image.matteFloodfill 1st arg must be Color");
			return false;
		}

		uint32 opacity;
		int32 x;
		int32 y;
		uint method;
		if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv+1, "uiiu", &opacity, &x, &y, &method)) return false;

		IMTRY()
			_imgStub->matteFloodfill(*target, opacity, x, y, (::Magick::PaintMethod)method);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_medianFilter(uintN argc, jsval *argv, jsval *rval)
	{
		if(0 != argc && 1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.medianFilter must be called with 0 or 1 arg");
			return false;
		}

		if(!argc)
		{
			IMTRY()
				_imgStub->medianFilter();
			IMCATCH(return false);
		}
		else
		{
			jsdouble radius;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &radius)) return false;

			IMTRY()
				_imgStub->medianFilter(radius);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_minify(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->minify();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_modulate(uintN argc, jsval *argv, jsval *rval)
	{
		if(3 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.modulate must be called with 3 args");
			return false;
		}

		double brightness;
		double saturation;
		double hue;
		if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "ddd", &brightness, &saturation, &hue)) return false;

		IMTRY()
			_imgStub->modulate(brightness, saturation, hue);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_motionBlur(uintN argc, jsval *argv, jsval *rval)
	{
		if(3 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.motionBlur must be called with 3 args");
			return false;
		}

		double radius, sigma, angle;
		if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "ddd", &radius, &sigma, &angle)) return false;

		IMTRY()
			_imgStub->motionBlur(radius, sigma, angle);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_negate(uintN argc, jsval *argv, jsval *rval)
	{
		if(0 != argc && 1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.negate must be called with 0 or 1 args");
			return false;
		}

		if(!argc)
		{
			IMTRY()
				_imgStub->negate();
			IMCATCH(return false);
		}
		else
		{
			JSBool grayscale;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &grayscale)) return false;

			IMTRY()
				_imgStub->negate(grayscale);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_normalize(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->normalize();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_oilPaint(uintN argc, jsval *argv, jsval *rval)
	{
		if(0 != argc && 1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.oilPaint must be called with 0 or 1 args");
			return false;
		}

		if(!argc)
		{
			IMTRY()
				_imgStub->oilPaint();
			IMCATCH(return false);
		}
		else
		{
			jsdouble radius;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &radius)) return false;

			IMTRY()
				_imgStub->oilPaint(radius);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_opacity(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.opacity must be called with 1 arg");
			return false;
		}

		uint32 opacity;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &opacity)) return false;

		IMTRY()
			_imgStub->opacity(opacity);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	bool Image::call_opaque(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.opaque must be called with 2 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Color> opaqueColor, penColor;

		if(!impl::colorFromJsv(argv[0], opaqueColor)) return false;
		if(!impl::colorFromJsv(argv[1], penColor)) return false;

		if(!opaqueColor || !penColor)
		{
			JS_ReportError(ecx()->_jsCx, "Image.opaque 1st and 2nd args must be a Color");
			return false;
		}


		IMTRY()
			_imgStub->opaque(*opaqueColor, *penColor);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_ping(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.ping must be called with 1 args");
			return false;
		}

		char *spec;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &spec)) return false;


		IMTRY()
			_imgStub->ping(spec);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_quantize(uintN argc, jsval *argv, jsval *rval)
	{
		if(0 != argc && 1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.quantize must be called with 0 or 1 args");
			return false;
		}

		if(!argc)
		{
			IMTRY()
				_imgStub->quantize();
			IMCATCH(return false);
		}
		else
		{
			JSBool measureError;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &measureError)) return false;

			IMTRY()
				_imgStub->quantize(measureError);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_quantumOperator(uintN argc, jsval *argv, jsval *rval)
	{
		if(3 != argc && 7 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.quantumOperator must be called with 3 or 7 args");
			return false;
		}

		if(3 == argc)
		{
			uint32 channel;
			uint32 operator_;
			double rvalue;

			if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "uud", &channel, &operator_, &rvalue)) return false;

			IMTRY()
				_imgStub->quantumOperator((::Magick::ChannelType)channel, (::Magick::MagickEvaluateOperator)operator_, rvalue);
			IMCATCH(return false);
		}
		else
		{
			int32 x,y;
			uint32 columns, rows;
			uint32 channel;
			uint32 operator_;
			double rvalue;

			if(!JS_ConvertArguments(ecx()->_jsCx, 6, argv, "iiuuuud", &x, &y, &columns, &rows, &channel, &operator_, &rvalue)) return false;

			IMTRY()
				_imgStub->quantumOperator(x, y, columns, rows, (::Magick::ChannelType)channel, (::Magick::MagickEvaluateOperator)operator_, rvalue);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_process(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 > argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.process must be called with 1 or more args");
			return false;
		}

		char *name;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &name)) return false;
		argc--;
		argv++;


		std::vector<char *> argPointers;
		while(argc)
		{
			char *str;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &str)) return false;
			argPointers.push_back(str);
			argc--;
			argv++;
		}
		/*
			локализованная винда выдает сообщения в CP1251, обезьяна ругается
		*/
		IMTRY()
			if(argPointers.size())
			{
				_imgStub->process(name, argPointers.size(), (const char **)&argPointers[0]);
			}
			else
			{
				_imgStub->process(name, 0, NULL);
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_raise(uintN argc, jsval *argv, jsval *rval)
	{
		if(0 != argc && 1 != argc && 2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.raise must be called with 0-2 args");
			return false;
		}

		if(!argc)
		{
			IMTRY()
				_imgStub->raise();
			IMCATCH(return false);
		}
		else
		{
			boost::shared_ptr< ::Magick::Geometry> geometry;
			if(!impl::geometryFromJsv(argv[0], geometry)) return false;
			if(!geometry)
			{
				JS_ReportError(ecx()->_jsCx, "Image.raise 1st arg must be Geometry");
				return false;
			}

			if(1 == argc)
			{
				IMTRY()
					_imgStub->raise(*geometry);
				IMCATCH(return false);
			}
			else
			{
				JSBool raisedFlag;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "b", &raisedFlag)) return false;

				IMTRY()
					_imgStub->raise(*geometry, raisedFlag);
				IMCATCH(return false);
			}
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_randomThreshold(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.randomThreshold must be called with 1 arg");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.randomThreshold 1st arg must be Geometry");
			return false;
		}

		IMTRY()
			_imgStub->randomThreshold(*geometry);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	bool Image::call_randomThresholdChannel(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.randomThreshold must be called with 2 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.randomThreshold 1st arg must be Geometry");
			return false;
		}

		uint32 channel;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "u", &channel)) return false;

		IMTRY()
			_imgStub->randomThresholdChannel(*geometry, (::Magick::ChannelType)channel);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	bool Image::call_read(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc && 2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.read must be called with 1 or 2 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		char *spec;
		if(1==argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &spec)) return false;
		}
		else
		{
			if(!impl::geometryFromJsv(argv[0], geometry)) return false;
			if(!geometry)
			{
				JS_ReportError(ecx()->_jsCx, "Image.randomThreshold 1st arg must be Geometry");
				return false;
			}
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "s", &spec)) return false;
		}


		IMTRY()
			if(geometry)
			{
				_imgStub->read(*geometry, spec);
			}
			else
			{
				_imgStub->read(spec);
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_reduceNoise(uintN argc, jsval *argv, jsval *rval)
	{
		if(0 != argc && 1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.reduceNoise must be called with 0 or 1 arg");
			return false;
		}
		if(!argc)
		{
			IMTRY()
				_imgStub->reduceNoise();
			IMCATCH(return false);
		}
		else
		{
			jsdouble order;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &order)) return false;

			IMTRY()
				_imgStub->reduceNoise(order);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_resize(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.resize must be called with 1 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.resize 1st arg must be Geometry");
			return false;
		}

		IMTRY()
			_imgStub->resize(*geometry);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	bool Image::call_roll(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc && 2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.roll must be called with 1 or 2 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		uint32 columns, rows;
		if(1 == argc)
		{
			if(!impl::geometryFromJsv(argv[0], geometry)) return false;
			if(!geometry)
			{
				JS_ReportError(ecx()->_jsCx, "Image.roll 1st arg must be Geometry");
				return false;
			}

		}
		else
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "uu", &columns, &rows)) return false;
		}

		if(geometry)
		{
			IMTRY()
				_imgStub->roll(*geometry);
			IMCATCH(return false);
		}
		else
		{
			IMTRY()
				_imgStub->roll(columns, rows);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_rotate(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.rotate must be called with 1 arg");
			return false;
		}

		jsdouble degrees;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &degrees)) return false;

		IMTRY()
			_imgStub->rotate(degrees);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_sample(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.sample must be called with 1 arg");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.sample 1st arg must be Geometry");
			return false;
		}

		IMTRY()
			_imgStub->sample(*geometry);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_scale(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.scale must be called with 1 arg");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.scale 1st arg must be Geometry");
			return false;
		}

		IMTRY()
			_imgStub->scale(*geometry);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_segment(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble clusterThreshold = 1.0;
		jsdouble smoothingThreshold = 1.5;

		if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &clusterThreshold, &smoothingThreshold))
			{
				return false;
			}
		}
		else if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &clusterThreshold))
			{
				return false;
			}
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->segment(clusterThreshold, smoothingThreshold);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	bool Image::call_shade(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble azimuth=30, elevation=30;
		bool colorShading = false;

		if(argc>=3)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "ddb", &azimuth, &elevation, &colorShading)) return false;
		}
		if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &azimuth, &elevation)) return false;
		}
		else if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &azimuth)) return false;
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->shade(azimuth, elevation, colorShading);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_sharpen(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble radius=0;
		jsdouble sigma=1.0;

		if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &radius, &sigma)) return false;
		}
		else if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &radius)) return false;
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->sharpen(radius, sigma);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_sharpenChannel(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "Image.sharpenChannel must be called with 1,2 or 3 args");
			return false;
		}

		uint32 channel;
		jsdouble radius=0;
		jsdouble sigma=1.0;

		if(argc>=3)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "udd", &channel, &radius, &sigma)) return false;
		}
		else if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ud", &channel, &radius)) return false;
		}
		else
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &channel)) return false;
		}

		IMTRY()
			_imgStub->sharpenChannel((::Magick::ChannelType)channel, radius, sigma);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_shave(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.shave must be called with 1 arg");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.shave 1st arg must be Geometry");
			return false;
		}

		IMTRY()
			_imgStub->shave(*geometry);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_shear(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 2)
		{
			JS_ReportError(ecx()->_jsCx, "Image.shear must be called 2 args");
			return false;
		}

		jsdouble xShearAngle, yShearAngle;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &xShearAngle, &yShearAngle)) return false;

		IMTRY()
			_imgStub->shear(xShearAngle, yShearAngle);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_sigmoidalContrast(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc != 2 && argc != 3)
		{
			JS_ReportError(ecx()->_jsCx, "Image.sigmoidalContrast must be called 2 or 3 args");
			return false;
		}

		uint32 sharpen;
		jsdouble contrast;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ud", &sharpen, &contrast)) return false;
		
		if(2 == argc)
		{
			IMTRY()
				_imgStub->sigmoidalContrast(sharpen, contrast);
			IMCATCH(return false);
		}
		else
		{
			jsdouble midpoint;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+2, "d", &midpoint)) return false;
			IMTRY()
				_imgStub->sigmoidalContrast(sharpen, contrast, midpoint);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_solarize(uintN argc, jsval *argv, jsval *rval)
	{
		if(!argc)
		{
			IMTRY()
				_imgStub->solarize();
			IMCATCH(return false);
		}
		else
		{
			jsdouble factor;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &factor)) return false;
			IMTRY()
				_imgStub->solarize(factor);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_splice(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.splice must be called with 1 arg");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.splice 1st arg must be Geometry");
			return false;
		}

		IMTRY()
			_imgStub->splice(*geometry);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_spread(uintN argc, jsval *argv, jsval *rval)
	{
		if(!argc)
		{
			IMTRY()
				_imgStub->spread();
			IMCATCH(return false);
		}
		else
		{
			uint32 amount;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &amount)) return false;
			IMTRY()
				_imgStub->spread(amount);
			IMCATCH(return false);
		}

		*rval = JSVAL_VOID;
		return true;
	}

	bool Image::call_sparseColor(uintN argc, jsval *argv, jsval *rval)
	{
		if(3 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.sparseColor must be called with 3 args");
			return false;
		}

		uint32 channel, method;
		if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "uu", &channel, &method)) return false;

		std::vector<jsdouble> arguments;
		JSContext *cx = ecx()->_jsCx;

		JSObject *jso = JSVAL_TO_OBJECT(argv[2]);
		if(JS_IsArrayObject(cx, jso))
		{
			jsuint len;
			if(!JS_GetArrayLength(cx, jso, &len)) return false;

			arguments.resize(len);
			for(jsuint i(0); i<len; i++)
			{
				jsval jsv;
				if(!JS_GetElement(cx, jso, i, &jsv)) return false;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "d", &arguments[i])) return false;
			}
		}
		else
		{
			JS_ReportError(cx, "Image.sparseColor 3rd arg must be a Array of numbers");
			return false;
		}

		IMTRY()
			if(arguments.size())
			{
				_imgStub->sparseColor((::Magick::ChannelType)channel, (::Magick::SparseColorMethod)method, arguments.size(), &arguments[0]);
			}
			else
			{
				_imgStub->sparseColor((::Magick::ChannelType)channel, (::Magick::SparseColorMethod)method, 0, NULL);
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_stegano(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.stegano must be called with 1 arg");
			return false;
		}

		Image *image = impl::imageFromJsv(argv[0]);
		if(!image)
		{
			JS_ReportError(ecx()->_jsCx, "Image.stegano 1st arg must be Image");
			return false;
		}

		IMTRY()
			_imgStub->stegano(image->getImpl());
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_stereo(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.stereo must be called with 1 arg");
			return false;
		}

		Image *image = impl::imageFromJsv(argv[0]);
		if(!image)
		{
			JS_ReportError(ecx()->_jsCx, "Image.stereo 1st arg must be Image");
			return false;
		}

		IMTRY()
			_imgStub->stereo(image->getImpl());
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_swirl(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.swirl must be called with 1 arg");
			return false;
		}

		jsdouble degrees;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &degrees)) return false;

		IMTRY()
			_imgStub->swirl(degrees);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_texture(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.texture must be called with 1 arg");
			return false;
		}

		Image *image = impl::imageFromJsv(argv[0]);
		if(!image)
		{
			JS_ReportError(ecx()->_jsCx, "Image.texture 1st arg must be Image");
			return false;
		}

		IMTRY()
			_imgStub->texture(image->getImpl());
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_threshold(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.threshold must be called with 1 arg");
			return false;
		}

		jsdouble threshold;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &threshold)) return false;

		IMTRY()
			_imgStub->threshold(threshold);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_transform(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc && 2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transform must be called with 1 or 2 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> imageGeometry, cropGeometry;
		if(!impl::geometryFromJsv(argv[0], imageGeometry)) return false;
		if(!imageGeometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transform 1st arg must be Geometry");
			return false;
		}
		if(argc > 1)
		{
			if(!impl::geometryFromJsv(argv[1], cropGeometry)) return false;
			if(!cropGeometry)
			{
				JS_ReportError(ecx()->_jsCx, "Image.transform 2nd arg must be Geometry");
				return false;
			}
		}



		IMTRY()
			if(cropGeometry)
			{
				_imgStub->transform(*imageGeometry, *cropGeometry);
			}
			else
			{
				_imgStub->transform(*imageGeometry);
			}
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_transparent(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transparent must be called with 1 arg");
			return false;
		}

		boost::shared_ptr< ::Magick::Color> color;
		if(!impl::colorFromJsv(argv[0], color)) return false;
		if(!color)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transparent 1st arg must be Color");
			return false;
		}

		IMTRY()
			_imgStub->transparent(*color);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_transparentChroma(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transparentChroma must be called with 2 args");
			return false;
		}

		boost::shared_ptr< ::Magick::Color> colorLow, colorHigh;
		if(!impl::colorFromJsv(argv[0], colorLow)) return false;
		if(!colorLow)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transparentChroma 1st arg must be Color");
			return false;
		}
		if(!impl::colorFromJsv(argv[1], colorHigh)) return false;
		if(!colorHigh)
		{
			JS_ReportError(ecx()->_jsCx, "Image.transparentChroma 2nd arg must be Color");
			return false;
		}

		IMTRY()
			_imgStub->transparentChroma(*colorLow, *colorHigh);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_trim(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->trim();
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_type(uintN argc, jsval *argv, jsval *rval)
	{
		uint32 type;
		if(argc)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &type)) return false;

			IMTRY()
				_imgStub->type((::Magick::ImageType)type);
			IMCATCH(return false);
		}

		IMTRY()
			type = _imgStub->type();
		IMCATCH(return false);

		*rval = INT_TO_JSVAL(type);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_unsharpmask(uintN argc, jsval *argv, jsval *rval)
	{
		if(4 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.unsharpmask must be called with 4 args");
		}

		jsdouble raduis, sigma, amount, threshold;
		if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "dddd", &raduis, &sigma, &amount, &threshold)) return false;

		IMTRY()
			_imgStub->unsharpmask(raduis, sigma, amount, threshold);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	bool Image::call_unsharpmaskChannel(uintN argc, jsval *argv, jsval *rval)
	{
		if(5 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.unsharpmaskChannel must be called with 5 args");
		}

		uint32 channel;
		jsdouble raduis, sigma, amount, threshold;
		if(!JS_ConvertArguments(ecx()->_jsCx, 5, argv, "udddd", &channel, &raduis, &sigma, &amount, &threshold)) return false;

		IMTRY()
			_imgStub->unsharpmaskChannel((::Magick::ChannelType)channel, raduis, sigma, amount, threshold);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_wave(uintN argc, jsval *argv, jsval *rval)
	{
		jsdouble amplitude = 25.0;
		jsdouble waveLength = 150.0;

		if(argc>=2)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "dd", &amplitude, &waveLength)) return false;
		}
		else if(argc>=1)
		{
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "d", &amplitude)) return false;
		}
		else
		{
			//nothing, ok
		}

		IMTRY()
			_imgStub->wave(amplitude, waveLength);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_write(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 > argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.write must be called with 1 arg");
			return false;
		}

		char *spec;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &spec)) return false;

		IMTRY()
			_imgStub->write(spec);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_zoom(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Image.zoom must be called with 1 arg");
			return false;
		}

		boost::shared_ptr< ::Magick::Geometry> geometry;
		if(!impl::geometryFromJsv(argv[0], geometry)) return false;
		if(!geometry)
		{
			JS_ReportError(ecx()->_jsCx, "Image.zoom 1st arg must be Geometry");
			return false;
		}

		IMTRY()
			_imgStub->zoom(*geometry);
		IMCATCH(return false);

		*rval = JSVAL_VOID;
		return true;
	}






	//////////////////////////////////////////////////////////////////////////
	bool Image::call_statistics(uintN argc, jsval *argv, jsval *rval)
	{
		::Magick::Image::ImageStatistics statistics = {};
		IMTRY()
			_imgStub->statistics(&statistics);
		IMCATCH(return false);


		struct {bool operator()(::Magick::Image::ImageChannelStatistics &s, jsval *rval)
		{
			JSObject *obj;
			if(!(obj = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL))) return false;

			jsval jsv;

			if(!JS_NewDoubleValue(ecx()->_jsCx, s.maximum, &jsv)) return false;
			if(!JS_DefineProperty(ecx()->_jsCx, obj, "maximum", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;
			if(!JS_NewDoubleValue(ecx()->_jsCx, s.minimum, &jsv)) return false;
			if(!JS_DefineProperty(ecx()->_jsCx, obj, "minimum", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;
			if(!JS_NewDoubleValue(ecx()->_jsCx, s.mean, &jsv)) return false;
			if(!JS_DefineProperty(ecx()->_jsCx, obj, "mean", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;
			if(!JS_NewDoubleValue(ecx()->_jsCx, s.standard_deviation, &jsv)) return false;
			if(!JS_DefineProperty(ecx()->_jsCx, obj, "stddev", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;
			if(!JS_NewDoubleValue(ecx()->_jsCx, s.variance, &jsv)) return false;
			if(!JS_DefineProperty(ecx()->_jsCx, obj, "variance", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;
			if(!JS_NewDoubleValue(ecx()->_jsCx, s.kurtosis, &jsv)) return false;
			if(!JS_DefineProperty(ecx()->_jsCx, obj, "kurtosis", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;
			if(!JS_NewDoubleValue(ecx()->_jsCx, s.skewness, &jsv)) return false;
			if(!JS_DefineProperty(ecx()->_jsCx, obj, "skewness", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;

			*rval = OBJECT_TO_JSVAL(obj);
			return true;
		}}oneChannel;

		JSObject *obj;
		if(!(obj = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL))) return false;
		jsval jsv;

		if(!oneChannel(statistics.red, &jsv)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, obj, "red", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;
		if(!oneChannel(statistics.green, &jsv)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, obj, "green", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;
		if(!oneChannel(statistics.blue, &jsv)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, obj, "blue", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;
		if(!oneChannel(statistics.opacity, &jsv)) return false;
		if(!JS_DefineProperty(ecx()->_jsCx, obj, "opacity", jsv, NULL, NULL, JSPROP_ENUMERATE)) return false;

		*rval = OBJECT_TO_JSVAL(obj);
		return true;
	}


	bool Image::call_attributes(uintN argc, jsval *argv, jsval *rval)
	{
		if(!_attributes)
		{
			_attributes = new ImageAttributes(this);
			if(!JS_SetReservedSlot(ecx()->_jsCx, _js, 0, _attributes->thisJsval()))
			{
				_attributes = NULL;
				return false;
			}
		}
		
		*rval = _attributes->thisJsval();
		return true;
	}






	//////////////////////////////////////////////////////////////////////////
	bool Image::call_modifyImage(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->modifyImage();
		IMCATCH(return false);
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Image::call_registerId(uintN argc, jsval *argv, jsval *rval)
	{
		int32 value;
		IMTRY()
			value = _imgStub->registerId();
		IMCATCH(return false);
		*rval = INT_TO_JSVAL(value);
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Image::call_unregisterId(uintN argc, jsval *argv, jsval *rval)
	{
		IMTRY()
			_imgStub->unregisterId();
		IMCATCH(return false);
		*rval = JSVAL_VOID;
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	::Magick::Image &Image::getImpl() const
	{
		return *_impl;
	}
















	//////////////////////////////////////////////////////////////////////////
	ImageConstructor::ImageConstructor()
		: JsObject(true, "ImageConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ImageConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		Image *img = new Image();
		if(!img)
		{
			JS_ReportOutOfMemory(ecx()->_jsCx);
			return JS_FALSE;
		}

		if(!img->construct(argc, argv))
		{
			return JS_FALSE;
		}

		*rval = img->thisJsval();
		return JS_TRUE;
	}

}}

