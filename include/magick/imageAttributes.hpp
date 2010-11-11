#ifndef _ccms_magick_imageAttributes_hpp_
#define _ccms_magick_imageAttributes_hpp_

#include "scripter/jsObject.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class ImageAttributes
		: public JsObject
	{
	public:
		ImageAttributes(Image *img);
		~ImageAttributes();


		bool call_image(uintN argc, jsval *argv, jsval *rval);
		bool call_adjoin(uintN argc, jsval *argv, jsval *rval);
		bool call_antiAlias(uintN argc, jsval *argv, jsval *rval);
		bool call_animationDelay(uintN argc, jsval *argv, jsval *rval);
		bool call_animationIterations(uintN argc, jsval *argv, jsval *rval);
		bool call_attribute(uintN argc, jsval *argv, jsval *rval);
		bool call_backgroundColor(uintN argc, jsval *argv, jsval *rval);
		bool call_backgroundTexture(uintN argc, jsval *argv, jsval *rval);
		bool call_baseColumns(uintN argc, jsval *argv, jsval *rval);
		bool call_baseFilename(uintN argc, jsval *argv, jsval *rval);
		bool call_baseRows(uintN argc, jsval *argv, jsval *rval);
		bool call_borderColor(uintN argc, jsval *argv, jsval *rval);
		bool call_boundingBox(uintN argc, jsval *argv, jsval *rval);
		bool call_boxColor(uintN argc, jsval *argv, jsval *rval);
		bool call_chromaBluePrimary(uintN argc, jsval *argv, jsval *rval);
		bool call_chromaGreenPrimary(uintN argc, jsval *argv, jsval *rval);
		bool call_chromaRedPrimary(uintN argc, jsval *argv, jsval *rval);
		bool call_chromaWhitePoint(uintN argc, jsval *argv, jsval *rval);
		bool call_classType(uintN argc, jsval *argv, jsval *rval);
		bool call_clipMask(uintN argc, jsval *argv, jsval *rval);
		bool call_colorFuzz(uintN argc, jsval *argv, jsval *rval);
		bool call_colorMap(uintN argc, jsval *argv, jsval *rval);
		bool call_colorMapSize(uintN argc, jsval *argv, jsval *rval);
		bool call_colorSpace(uintN argc, jsval *argv, jsval *rval);
		bool call_colorspaceType(uintN argc, jsval *argv, jsval *rval);
		bool call_columns(uintN argc, jsval *argv, jsval *rval);
		bool call_compressType(uintN argc, jsval *argv, jsval *rval);
		bool call_debug(uintN argc, jsval *argv, jsval *rval);
		bool call_defineValue(uintN argc, jsval *argv, jsval *rval);
		bool call_defineSet(uintN argc, jsval *argv, jsval *rval);
		bool call_density(uintN argc, jsval *argv, jsval *rval);
		bool call_depth(uintN argc, jsval *argv, jsval *rval);
		bool call_directory(uintN argc, jsval *argv, jsval *rval);
		bool call_endian(uintN argc, jsval *argv, jsval *rval);
		bool call_exifProfile(uintN argc, jsval *argv, jsval *rval);
		bool call_fileName(uintN argc, jsval *argv, jsval *rval);
		bool call_fileSize(uintN argc, jsval *argv, jsval *rval);
		bool call_fillColor(uintN argc, jsval *argv, jsval *rval);
		bool call_fillRule(uintN argc, jsval *argv, jsval *rval);
		bool call_fillPattern(uintN argc, jsval *argv, jsval *rval);
		bool call_filterType(uintN argc, jsval *argv, jsval *rval);
		bool call_font(uintN argc, jsval *argv, jsval *rval);
		bool call_fontPointsize(uintN argc, jsval *argv, jsval *rval);
		bool call_fontTypeMetrics(uintN argc, jsval *argv, jsval *rval);
		bool call_format(uintN argc, jsval *argv, jsval *rval);
		bool call_geometry(uintN argc, jsval *argv, jsval *rval);
		bool call_gifDisposeMethod(uintN argc, jsval *argv, jsval *rval);
		bool call_iccColorProfile(uintN argc, jsval *argv, jsval *rval);
		bool call_interlaceType(uintN argc, jsval *argv, jsval *rval);
		bool call_iptcProfile(uintN argc, jsval *argv, jsval *rval);
		bool call_isValid(uintN argc, jsval *argv, jsval *rval);
		bool call_lineWidth(uintN argc, jsval *argv, jsval *rval);
		bool call_magick(uintN argc, jsval *argv, jsval *rval);
		bool call_matte(uintN argc, jsval *argv, jsval *rval);
		bool call_matteColor(uintN argc, jsval *argv, jsval *rval);
		bool call_meanErrorPerPixel(uintN argc, jsval *argv, jsval *rval);
		bool call_modulusDepth(uintN argc, jsval *argv, jsval *rval);
		bool call_montageGeometry(uintN argc, jsval *argv, jsval *rval);
		bool call_monochrome(uintN argc, jsval *argv, jsval *rval);
		bool call_normalizedMaxError(uintN argc, jsval *argv, jsval *rval);
		bool call_normalizedMeanError(uintN argc, jsval *argv, jsval *rval);
		bool call_orientation(uintN argc, jsval *argv, jsval *rval);
		bool call_page(uintN argc, jsval *argv, jsval *rval);
		bool call_penColor(uintN argc, jsval *argv, jsval *rval);
		bool call_penTexture(uintN argc, jsval *argv, jsval *rval);
		bool call_pixelColor(uintN argc, jsval *argv, jsval *rval);
		bool call_profile(uintN argc, jsval *argv, jsval *rval);
		bool call_quality(uintN argc, jsval *argv, jsval *rval);
		bool call_quantizeColors(uintN argc, jsval *argv, jsval *rval);
		bool call_quantizeColorSpace(uintN argc, jsval *argv, jsval *rval);
		bool call_quantizeDither(uintN argc, jsval *argv, jsval *rval);
		bool call_quantizeTreeDepth(uintN argc, jsval *argv, jsval *rval);
		bool call_renderingIntent(uintN argc, jsval *argv, jsval *rval);
		bool call_resolutionUnits(uintN argc, jsval *argv, jsval *rval);
		bool call_rows(uintN argc, jsval *argv, jsval *rval);
		bool call_scene(uintN argc, jsval *argv, jsval *rval);
		bool call_signature(uintN argc, jsval *argv, jsval *rval);
		bool call_size(uintN argc, jsval *argv, jsval *rval);
		bool call_strokeAntiAlias(uintN argc, jsval *argv, jsval *rval);
		bool call_strokeColor(uintN argc, jsval *argv, jsval *rval);
		bool call_strokeDashArray(uintN argc, jsval *argv, jsval *rval);
		bool call_strokeDashOffset(uintN argc, jsval *argv, jsval *rval);
		bool call_strokeLineCap(uintN argc, jsval *argv, jsval *rval);
		bool call_strokeLineJoin(uintN argc, jsval *argv, jsval *rval);
		bool call_strokeMiterLimit(uintN argc, jsval *argv, jsval *rval);
		bool call_strokePattern(uintN argc, jsval *argv, jsval *rval);
		bool call_strokeWidth(uintN argc, jsval *argv, jsval *rval);
		bool call_subImage(uintN argc, jsval *argv, jsval *rval);
		bool call_subRange(uintN argc, jsval *argv, jsval *rval);
		bool call_tileName(uintN argc, jsval *argv, jsval *rval);
		bool call_totalColors(uintN argc, jsval *argv, jsval *rval);
		bool call_transformOrigin(uintN argc, jsval *argv, jsval *rval);
		bool call_transformRotation(uintN argc, jsval *argv, jsval *rval);
		bool call_transformReset(uintN argc, jsval *argv, jsval *rval);
		bool call_transformScale(uintN argc, jsval *argv, jsval *rval);
		bool call_transformSkewX(uintN argc, jsval *argv, jsval *rval);
		bool call_transformSkewY(uintN argc, jsval *argv, jsval *rval);
		bool call_verbose(uintN argc, jsval *argv, jsval *rval);
		bool call_view(uintN argc, jsval *argv, jsval *rval);
		bool call_virtualPixelMethod(uintN argc, jsval *argv, jsval *rval);
		bool call_xResolution(uintN argc, jsval *argv, jsval *rval);
		bool call_yResolution(uintN argc, jsval *argv, jsval *rval);


		//samplingFactor
		//type from Image


	private:
		bool jsRegister();
		Image *_img;
		::Magick::Image *iimpl();
	};

	typedef JsPtr<ImageAttributes> ImageAttributesPtr;
}}

#endif
