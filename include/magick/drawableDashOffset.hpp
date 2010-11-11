#ifndef _ccms_magick_drawableDashOffset_hpp_
#define _ccms_magick_drawableDashOffset_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableDashOffset
		: public DrawableBase
	{
	public:
		DrawableDashOffset(const double offset_);
		DrawableDashOffset(const DrawableDashOffset &from);
		~DrawableDashOffset();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_offset(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableDashOffset &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableDashOffset> _impl;

	};
	typedef JsPtr<DrawableDashOffset> DrawableDashOffsetPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableDashOffsetConstructor
		: public JsObject
	{
	public:
		DrawableDashOffsetConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
