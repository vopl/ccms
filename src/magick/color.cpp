#include "magick_pch.h"
#include "magick/color.hpp"
#include "magick/macroHelpers.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	Color::Color()
		: JsObject(true, "Color")
		, _impl(new ::Magick::Color)
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	Color::Color(const ::Magick::Color &color)
		: JsObject(true, "Color")
		, _impl(new ::Magick::Color(color))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	Color::Color(const char * x11color_)
		: JsObject(true, "Color")
		, _impl(new ::Magick::Color(x11color_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	Color::Color(uint32 red_,
		uint32 green_,
		uint32 blue_,
		uint32 alpha_)
		: JsObject(true, "Color")
		, _impl(new ::Magick::Color(red_, green_, blue_, alpha_))
	{
		(JSExceptionReporter)registerJs();
	}


	//////////////////////////////////////////////////////////////////////////
	Color::Color(const Color & color_)
		: JsObject(true, "Color")
		, _impl(new ::Magick::Color(color_.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_isValid(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = BOOLEAN_TO_JSVAL(_impl->isValid());
		}
		else
		{
			JSBool v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "b", &v)) return false;
			_impl->isValid(v);
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_string(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			IMTRY()
				std::string s = *_impl;
				*vp = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, s.data(), s.size()));
			IMCATCH(*vp = STRING_TO_JSVAL(JS_NewStringCopyZ(ecx()->_jsCx, "value acquiring failed")))

		}
		else
		{
			char *v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &v)) return false;

			IMTRY()
				*_impl = v;
			IMCATCH(return false)
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_intensity(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->intensity(), vp)) return false;
		}
		else
		{

		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_redQuantum(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->redQuantum());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->redQuantum(v);
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_greenQuantum(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->greenQuantum());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->greenQuantum(v);
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_blueQuantum(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->blueQuantum());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->blueQuantum(v);
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_alphaQuantum(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->alphaQuantum());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->alphaQuantum(v);
		}
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::call_hsl(uintN argc, jsval *argv, jsval *rval)
	{
		if(3 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Color.hls must be called with 3 args");
		}

		jsdouble h,s,l;
		if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "ddd", &h, &s, &l)) return false;

		::Magick::ColorHSL c(h, s, l);
		*_impl = c;

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_hue(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorHSL c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.hue(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorHSL c(*_impl);
			c.hue(v);
			*_impl = c;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_saturation(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorHSL c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.saturation(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorHSL c(*_impl);
			c.saturation(v);
			*_impl = c;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_luminosity(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorHSL c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.luminosity(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorHSL c(*_impl);
			c.luminosity(v);
			*_impl = c;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_gray(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorGray c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.shade(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorGray c(*_impl);
			c.shade(v);
			*_impl = c;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_mono(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorMono c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.mono(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorMono c(*_impl);
			c.mono(v);
			*_impl = c;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::call_rgb(uintN argc, jsval *argv, jsval *rval)
	{
		if(3 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Color.rgb must be called with 3 args");
		}

		jsdouble r,g,b;
		if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "ddd", &r, &g, &b)) return false;

		::Magick::ColorRGB c(r,g,b);
		*_impl = c;

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_red(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorRGB c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.red(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorRGB c(*_impl);
			c.red(v);
			*_impl = c;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_green(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorRGB c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.green(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorRGB c(*_impl);
			c.green(v);
			*_impl = c;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_blue(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorRGB c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.blue(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorRGB c(*_impl);
			c.blue(v);
			*_impl = c;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::call_rgba(uintN argc, jsval *argv, jsval *rval)
	{
		if(4 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Color.rgba must be called with 4 args");
		}

		jsdouble r,g,b,a;
		if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "dddd", &r, &g, &b, &a)) return false;

		::Magick::ColorRGB c(r,g,b);
		*_impl = c;
		_impl->alpha(a);

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_alpha(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->alpha(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			_impl->alpha(v);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::call_yuv(uintN argc, jsval *argv, jsval *rval)
	{
		if(3 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "Color.rgb must be called with 3 args");
		}

		jsdouble y,u,v;
		if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "ddd", &y, &u, &v)) return false;

		::Magick::ColorYUV c(y,u,v);
		*_impl = c;

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_y(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorYUV c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.y(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorYUV c(*_impl);
			c.y(v);
			*_impl = c;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_u(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorYUV c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.u(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorYUV c(*_impl);
			c.u(v);
			*_impl = c;
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	bool Color::xetter_v(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			::Magick::ColorYUV c(*_impl);
			if(!JS_NewDoubleValue(ecx()->_jsCx, c.v(), vp)) return false;
		}
		else
		{
			jsdouble v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &v)) return false;
			::Magick::ColorYUV c(*_impl);
			c.v(v);
			*_impl = c;
		}

		return true;
	}







	//////////////////////////////////////////////////////////////////////////
	bool Color::registerJs()
	{
		if(!jsRegisterProp("isValid", &Color::xetter_isValid, false)) return false;
		if(!jsRegisterProp("string", &Color::xetter_string, false)) return false;
		if(!jsRegisterProp("intensity", &Color::xetter_intensity, false)) return false;

		if(!jsRegisterProp("redQuantum", &Color::xetter_redQuantum, false)) return false;
		if(!jsRegisterProp("greenQuantum", &Color::xetter_greenQuantum, false)) return false;
		if(!jsRegisterProp("blueQuantum", &Color::xetter_blueQuantum, false)) return false;
		if(!jsRegisterProp("alphaQuantum", &Color::xetter_alphaQuantum, false)) return false;


		if(!jsRegisterMeth("hsl", &Color::call_hsl, 3)) return false;
		if(!jsRegisterProp("hue", &Color::xetter_hue, false)) return false;
		if(!jsRegisterProp("saturation", &Color::xetter_saturation, false)) return false;
		if(!jsRegisterProp("luminosity", &Color::xetter_luminosity, false)) return false;

		if(!jsRegisterProp("shade", &Color::xetter_gray, false)) return false;
		if(!jsRegisterProp("mono", &Color::xetter_mono, false)) return false;

		if(!jsRegisterMeth("rgb", &Color::call_rgb, 3)) return false;
		if(!jsRegisterProp("red", &Color::xetter_red, false)) return false;
		if(!jsRegisterProp("green", &Color::xetter_green, false)) return false;
		if(!jsRegisterProp("blue", &Color::xetter_blue, false)) return false;

		if(!jsRegisterMeth("rgba", &Color::call_rgba, 4)) return false;
		if(!jsRegisterProp("alpha", &Color::xetter_alpha, false)) return false;

		if(!jsRegisterMeth("yuv", &Color::call_yuv, 3)) return false;
		if(!jsRegisterProp("y", &Color::xetter_y, false)) return false;
		if(!jsRegisterProp("u", &Color::xetter_u, false)) return false;
		if(!jsRegisterProp("v", &Color::xetter_v, false)) return false;

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::Color &Color::getImpl() const
	{
		return *_impl;
	}






	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	ColorConstructor::ColorConstructor()
		: JsObject(true, "ColorConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool ColorConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(!argc)
		{
			*rval = (new Color)->thisJsval();
		}
		else if(1 == argc)
		{
			if(JSVAL_IS_OBJECT(argv[0]) && !JSVAL_IS_NULL(argv[0]))
			{
				JSObject *jso = JSVAL_TO_OBJECT(argv[0]);
				JsObject *obj = JsObject::thisObj(jso);
				if(!obj)
				{
					JS_ReportError(ecx()->_jsCx, "ColorConstructor: arg must be an core object");
					return JS_FALSE;
				}

				Color *color = dynamic_cast<Color *>(obj);
				if(!color)
				{
					JS_ReportError(ecx()->_jsCx, "ColorConstructor: arg must be a Color object");
					return JS_FALSE;
				}
				*rval = (new Color(*color))->thisJsval();
			}
			else
			{
				char *strValue;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &strValue)) return false;
				*rval = (new Color(strValue))->thisJsval();
			}
		}
		else if(3 == argc)
		{
			uint32 vals[3];
			if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "uuu", vals+0, vals+1, vals+2)) return false;
			*rval = (new Color(vals[0], vals[1], vals[2]))->thisJsval();
		}
		else if(4 == argc)
		{
			uint32 vals[4];
			if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "uuuu", vals+0, vals+1, vals+2, vals+3)) return false;
			*rval = (new Color(vals[0], vals[1], vals[2], vals[3]))->thisJsval();
		}

		return JS_TRUE;
	}

}}
