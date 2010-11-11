#ifndef _ccms_magick_drawablePointSize_hpp_
#define _ccms_magick_drawablePointSize_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePointSize
		: public DrawableBase
	{
	public:
		DrawablePointSize(double pointSize_);
		DrawablePointSize(const DrawablePointSize &from);
		~DrawablePointSize();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_pointSize(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePointSize &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePointSize> _impl;

	};
	typedef JsPtr<DrawablePointSize> DrawablePointSizePtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePointSizeConstructor
		: public JsObject
	{
	public:
		DrawablePointSizeConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
