#ifndef _ccms_magick_image_hpp_
#define _ccms_magick_image_hpp_

#include "scripter/jsObject.hpp"
#include "magick/color.hpp"
#include "magick/geometry.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class ImageAttributes;

	//////////////////////////////////////////////////////////////////////////
	class Image
		: public JsObject
	{
	public:
		Image();
		Image(const ::Magick::Image &from);
		bool construct(uintN argc, jsval *argv);
		~Image();


		bool jsRegister();



		bool call_adaptiveBlur(uintN argc, jsval *argv, jsval *rval);
		bool call_adaptiveThreshold(uintN argc, jsval *argv, jsval *rval);
		bool call_addNoise(uintN argc, jsval *argv, jsval *rval);
		bool call_addNoiseChannel(uintN argc, jsval *argv, jsval *rval);
		bool call_affineTransform(uintN argc, jsval *argv, jsval *rval);
		bool call_annotate(uintN argc, jsval *argv, jsval *rval);
		bool call_blur(uintN argc, jsval *argv, jsval *rval);
		bool call_blurChannel(uintN argc, jsval *argv, jsval *rval);
		bool call_border(uintN argc, jsval *argv, jsval *rval);
		bool call_channel(uintN argc, jsval *argv, jsval *rval);
		bool call_channelDepth(uintN argc, jsval *argv, jsval *rval);
		bool call_charcoal(uintN argc, jsval *argv, jsval *rval);
		bool call_chop(uintN argc, jsval *argv, jsval *rval);
		bool call_cdl(uintN argc, jsval *argv, jsval *rval);
		bool call_colorize(uintN argc, jsval *argv, jsval *rval);
		bool call_colorMatrix(uintN argc, jsval *argv, jsval *rval);
		bool call_comment(uintN argc, jsval *argv, jsval *rval);
		bool call_compose(uintN argc, jsval *argv, jsval *rval);
		bool call_compare(uintN argc, jsval *argv, jsval *rval);
		bool call_composite(uintN argc, jsval *argv, jsval *rval);
		bool call_contrast(uintN argc, jsval *argv, jsval *rval);
		bool call_convolve(uintN argc, jsval *argv, jsval *rval);
		bool call_crop(uintN argc, jsval *argv, jsval *rval);
		bool call_cycleColormap(uintN argc, jsval *argv, jsval *rval);
		bool call_despeckle(uintN argc, jsval *argv, jsval *rval);
		bool call_distort(uintN argc, jsval *argv, jsval *rval);
		bool call_draw(uintN argc, jsval *argv, jsval *rval);
		bool call_edge(uintN argc, jsval *argv, jsval *rval);
		bool call_emboss(uintN argc, jsval *argv, jsval *rval);
		bool call_enhance(uintN argc, jsval *argv, jsval *rval);
		bool call_equalize(uintN argc, jsval *argv, jsval *rval);
		bool call_erase(uintN argc, jsval *argv, jsval *rval);
		bool call_extent(uintN argc, jsval *argv, jsval *rval);
		bool call_flip(uintN argc, jsval *argv, jsval *rval);
		bool call_floodFillColor(uintN argc, jsval *argv, jsval *rval);
		bool call_floodFillOpacity(uintN argc, jsval *argv, jsval *rval);
		bool call_floodFillTexture(uintN argc, jsval *argv, jsval *rval);
		bool call_flop(uintN argc, jsval *argv, jsval *rval);
		bool call_frame(uintN argc, jsval *argv, jsval *rval);
		bool call_fx(uintN argc, jsval *argv, jsval *rval);
		bool call_gamma(uintN argc, jsval *argv, jsval *rval);
		bool call_gaussianBlur(uintN argc, jsval *argv, jsval *rval);
		bool call_gaussianBlurChannel(uintN argc, jsval *argv, jsval *rval);
		bool call_getBase64(uintN argc, jsval *argv, jsval *rval);
		bool call_haldClut(uintN argc, jsval *argv, jsval *rval);
		bool call_implode(uintN argc, jsval *argv, jsval *rval);
		bool call_inverseFourierTransform(uintN argc, jsval *argv, jsval *rval);
		bool call_label(uintN argc, jsval *argv, jsval *rval);
		bool call_level(uintN argc, jsval *argv, jsval *rval);
		bool call_levelChannel(uintN argc, jsval *argv, jsval *rval);
		bool call_magick(uintN argc, jsval *argv, jsval *rval);
		bool call_magnify(uintN argc, jsval *argv, jsval *rval);
		bool call_map(uintN argc, jsval *argv, jsval *rval);
		bool call_matteFloodfill(uintN argc, jsval *argv, jsval *rval);
		bool call_medianFilter(uintN argc, jsval *argv, jsval *rval);
		bool call_minify(uintN argc, jsval *argv, jsval *rval);
		bool call_modulate(uintN argc, jsval *argv, jsval *rval);
		bool call_motionBlur(uintN argc, jsval *argv, jsval *rval);
		bool call_negate(uintN argc, jsval *argv, jsval *rval);
		bool call_normalize(uintN argc, jsval *argv, jsval *rval);
		bool call_oilPaint(uintN argc, jsval *argv, jsval *rval);
		bool call_opacity(uintN argc, jsval *argv, jsval *rval);
		bool call_opaque(uintN argc, jsval *argv, jsval *rval);
		bool call_ping(uintN argc, jsval *argv, jsval *rval);
		bool call_quantize(uintN argc, jsval *argv, jsval *rval);
		bool call_quantumOperator(uintN argc, jsval *argv, jsval *rval);
		bool call_process(uintN argc, jsval *argv, jsval *rval);
		bool call_raise(uintN argc, jsval *argv, jsval *rval);
		bool call_randomThreshold(uintN argc, jsval *argv, jsval *rval);
		bool call_randomThresholdChannel(uintN argc, jsval *argv, jsval *rval);
		bool call_read(uintN argc, jsval *argv, jsval *rval);
		bool call_reduceNoise(uintN argc, jsval *argv, jsval *rval);
		bool call_resize(uintN argc, jsval *argv, jsval *rval);
		bool call_roll(uintN argc, jsval *argv, jsval *rval);
		bool call_rotate(uintN argc, jsval *argv, jsval *rval);
		bool call_sample(uintN argc, jsval *argv, jsval *rval);
		bool call_scale(uintN argc, jsval *argv, jsval *rval);
		bool call_segment(uintN argc, jsval *argv, jsval *rval);
		bool call_shade(uintN argc, jsval *argv, jsval *rval);
		bool call_sharpen(uintN argc, jsval *argv, jsval *rval);
		bool call_sharpenChannel(uintN argc, jsval *argv, jsval *rval);
		bool call_shave(uintN argc, jsval *argv, jsval *rval);
		bool call_shear(uintN argc, jsval *argv, jsval *rval);
		bool call_sigmoidalContrast(uintN argc, jsval *argv, jsval *rval);
		bool call_solarize(uintN argc, jsval *argv, jsval *rval);
		bool call_splice(uintN argc, jsval *argv, jsval *rval);
		bool call_spread(uintN argc, jsval *argv, jsval *rval);
		bool call_sparseColor(uintN argc, jsval *argv, jsval *rval);
		bool call_stegano(uintN argc, jsval *argv, jsval *rval);
		bool call_stereo(uintN argc, jsval *argv, jsval *rval);
		bool call_swirl(uintN argc, jsval *argv, jsval *rval);
		bool call_texture(uintN argc, jsval *argv, jsval *rval);
		bool call_threshold(uintN argc, jsval *argv, jsval *rval);
		bool call_transform(uintN argc, jsval *argv, jsval *rval);
		bool call_transparent(uintN argc, jsval *argv, jsval *rval);
		bool call_transparentChroma(uintN argc, jsval *argv, jsval *rval);
		bool call_trim(uintN argc, jsval *argv, jsval *rval);
		bool call_type(uintN argc, jsval *argv, jsval *rval);
		bool call_unsharpmask(uintN argc, jsval *argv, jsval *rval);
		bool call_unsharpmaskChannel(uintN argc, jsval *argv, jsval *rval);
		bool call_wave(uintN argc, jsval *argv, jsval *rval);
		bool call_write(uintN argc, jsval *argv, jsval *rval);
		bool call_zoom(uintN argc, jsval *argv, jsval *rval);

		bool call_statistics(uintN argc, jsval *argv, jsval *rval);
		bool call_attributes(uintN argc, jsval *argv, jsval *rval);

		bool call_modifyImage(uintN argc, jsval *argv, jsval *rval);
		bool call_registerId(uintN argc, jsval *argv, jsval *rval);
		bool call_unregisterId(uintN argc, jsval *argv, jsval *rval);

		::Magick::Image &getImpl() const;
		private:
			boost::shared_ptr< ::Magick::Image> _impl;
			::Magick::Image *_imgStub;
			ImageAttributes *_attributes;

	};
	typedef JsPtr<Image> ImagePtr;


	//////////////////////////////////////////////////////////////////////////
	class ImageConstructor
		: public JsObject
	{
	public:
		ImageConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};


}}
#endif

