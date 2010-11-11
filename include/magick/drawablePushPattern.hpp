#ifndef _ccms_magick_drawablePushPattern_hpp_
#define _ccms_magick_drawablePushPattern_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePushPattern
		: public DrawableBase
	{
	public:
		DrawablePushPattern(const char *id_, int32 x_, int32 y_, uint32 width_, uint32 height_);
		DrawablePushPattern(const DrawablePushPattern &from);
		~DrawablePushPattern();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePushPattern &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePushPattern> _impl;

	};
	typedef JsPtr<DrawablePushPattern> DrawablePushPatternPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePushPatternConstructor
		: public JsObject
	{
	public:
		DrawablePushPatternConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
