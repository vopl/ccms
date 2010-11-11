#ifndef _ccms_magick_imageCnvtHelpers_hpp_
#define _ccms_magick_imageCnvtHelpers_hpp_

#include "magick/drawableBase.hpp"


namespace ccms{namespace magick{

	//////////////////////////////////////////////////////////////////////////
	namespace impl
	{
		inline bool geometryFromJsv(jsval jsv, boost::shared_ptr< ::Magick::Geometry> &geometry)
		{
			geometry.reset();

			if(JSVAL_IS_OBJECT(jsv))
			{
				JSObject *jso;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "o", &jso))return false;
				Geometry *geometryOwn = dynamic_cast<Geometry *>(JsObject::thisObj(jso));
				if(geometryOwn && !geometryOwn->getImpl().isValid())
				{
					JS_ReportError(ecx()->_jsCx, "Image::geometryFromJsv 1st arg must be a valid Geometry object");
					return false;
				}
				geometry.reset(new ::Magick::Geometry(geometryOwn->getImpl()));
			}
			else if(JSVAL_IS_STRING(jsv))
			{
				char *strGeometry;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &strGeometry))return false;
				IMTRY()
					geometry.reset(new ::Magick::Geometry(strGeometry));
				IMCATCH(return false);
				if(!geometry->isValid())
				{
					JS_ReportError(ecx()->_jsCx, "Image::geometryFromJsv 1st arg must be a valid Geometry string");
					return false;
				}
			}

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		inline bool colorFromJsv(jsval jsv, boost::shared_ptr< ::Magick::Color> &color)
		{
			color.reset();

			if(JSVAL_IS_OBJECT(jsv))
			{
				JSObject *jso;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "o", &jso))return false;
				Color *colorOwn = dynamic_cast<Color *>(JsObject::thisObj(jso));
				if(colorOwn && !colorOwn->getImpl().isValid())
				{
					JS_ReportError(ecx()->_jsCx, "Image::colorFromJsv 1st arg must be a valid Color object");
					return false;
				}
				color.reset(new ::Magick::Color(colorOwn->getImpl()));
			}
			else if(JSVAL_IS_STRING(jsv))
			{
				char *strColor;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &strColor))return false;
				IMTRY();
				color.reset(new ::Magick::Color(strColor));
				IMCATCH(return false);

				if(!color->isValid())
				{
					JS_ReportError(ecx()->_jsCx, "Image::colorFromJsv 1st arg must be a valid Color string");
					return false;
				}
			}

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		inline Image *imageFromJsv(jsval jsv)
		{
			JSObject *jso;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "o", &jso)) return false;
			Image *obj = dynamic_cast<Image *>(JsObject::thisObj(jso));

			return obj;
		}









		inline bool Drawable_jsv(JSObject *jso, ::Magick::Drawable &p)
		{
			JsObject *obj = JsObject::thisObj(jso);
			if(!obj)
			{
				JS_ReportError(ecx()->_jsCx, "Drawable is not valid");
				return false;
			}

			DrawableBase *pp = dynamic_cast<DrawableBase *>(obj);
			if(pp)
			{
				p = ::Magick::Drawable(pp->getBaseImpl());
				return true;
			}

			JS_ReportError(ecx()->_jsCx, "Drawable is not valid Path object");
			return false;
		}


		//////////////////////////////////////////////////////////////////////////
		inline bool DrawableList_jsv(jsval jsv, ::Magick::DrawableList &list)
		{
			//одна координата или массив координат
			if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
			{
				JS_ReportError(ecx()->_jsCx, "DrawableList arg must be a hash or array");
				return false;
			}

			JSObject *jso = JSVAL_TO_OBJECT(jsv);
			if(JS_IsArrayObject(ecx()->_jsCx, jso))
			{
				jsuint len;
				if(!JS_GetArrayLength(ecx()->_jsCx, jso, &len)) return false;

				for(jsuint i(0); i<len; i++)
				{
					if(!JS_GetElement(ecx()->_jsCx, jso, i, &jsv)) return false;

					if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
					{
						JS_ReportError(ecx()->_jsCx, "DrawableList arg %d must be a Path object", i);
						return false;
					}
					JSObject *jsoElement = JSVAL_TO_OBJECT(jsv);

					::Magick::Drawable p;
					if(!Drawable_jsv(jsoElement, p)) return false;
					list.push_back(p);
				}

			}
			else
			{
				::Magick::Drawable p;
				if(!Drawable_jsv(jso, p)) return false;
				list.push_back(p);
			}

			return true;
		}

	}
}}

#endif
