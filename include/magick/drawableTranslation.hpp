#ifndef _ccms_magick_drawableTranslation_hpp_
#define _ccms_magick_drawableTranslation_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableTranslation
		: public DrawableBase
	{
	public:
		DrawableTranslation(double x_, double y_);
		DrawableTranslation(const DrawableTranslation &from);
		~DrawableTranslation();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_x(jsval *vp, bool isGet);
		bool xetter_y(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableTranslation &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableTranslation> _impl;

	};
	typedef JsPtr<DrawableTranslation> DrawableTranslationPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableTranslationConstructor
		: public JsObject
	{
	public:
		DrawableTranslationConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
