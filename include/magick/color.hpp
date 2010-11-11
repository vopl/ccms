#ifndef _ccms_magick_color_hpp_
#define _ccms_magick_color_hpp_

#include "scripter/jsObject.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class Color
		: public JsObject
	{

	public:
		Color();
		Color(const ::Magick::Color &color);
		Color(const char * x11color_);
		Color(uint32 red_,
			uint32 green_,
			uint32 blue_,
			uint32 alpha_ = 0);
		Color(const Color & color_);

		bool xetter_isValid(jsval *vp, bool isGet);
		bool xetter_string(jsval *vp, bool isGet);
		bool xetter_intensity(jsval *vp, bool isGet);

		bool xetter_redQuantum(jsval *vp, bool isGet);
		bool xetter_greenQuantum(jsval *vp, bool isGet);
		bool xetter_blueQuantum(jsval *vp, bool isGet);
		bool xetter_alphaQuantum(jsval *vp, bool isGet);


		bool call_hsl(uintN argc, jsval *argv, jsval *rval);
		bool xetter_hue(jsval *vp, bool isGet);
		bool xetter_saturation(jsval *vp, bool isGet);
		bool xetter_luminosity(jsval *vp, bool isGet);

		bool xetter_gray(jsval *vp, bool isGet);
		bool xetter_mono(jsval *vp, bool isGet);

		bool call_rgb(uintN argc, jsval *argv, jsval *rval);
		bool xetter_red(jsval *vp, bool isGet);
		bool xetter_green(jsval *vp, bool isGet);
		bool xetter_blue(jsval *vp, bool isGet);

		bool call_rgba(uintN argc, jsval *argv, jsval *rval);
		bool xetter_alpha(jsval *vp, bool isGet);

		bool call_yuv(uintN argc, jsval *argv, jsval *rval);
		bool xetter_y(jsval *vp, bool isGet);
		bool xetter_u(jsval *vp, bool isGet);
		bool xetter_v(jsval *vp, bool isGet);

		bool registerJs();

		const ::Magick::Color &getImpl() const;

	private:
		boost::shared_ptr< ::Magick::Color> _impl;

	};
	typedef JsPtr<Color> ColorPtr;







	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	class ColorConstructor
		: public JsObject
	{
	public:
		ColorConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
