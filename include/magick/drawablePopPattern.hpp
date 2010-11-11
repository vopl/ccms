#ifndef _ccms_magick_drawablePopPattern_hpp_
#define _ccms_magick_drawablePopPattern_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePopPattern
		: public DrawableBase
	{
	public:
		DrawablePopPattern();
		~DrawablePopPattern();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePopPattern &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePopPattern> _impl;

	};
	typedef JsPtr<DrawablePopPattern> DrawablePopPatternPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePopPatternConstructor
		: public JsObject
	{
	public:
		DrawablePopPatternConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
