#ifndef _ccms_magick_drawableMiterLimit_hpp_
#define _ccms_magick_drawableMiterLimit_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableMiterLimit
		: public DrawableBase
	{
	public:
		DrawableMiterLimit(uint32 from);
		DrawableMiterLimit(const DrawableMiterLimit &from);
		~DrawableMiterLimit();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_miterLimit(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableMiterLimit &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableMiterLimit> _impl;

	};
	typedef JsPtr<DrawableMiterLimit> DrawableMiterLimitPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableMiterLimitConstructor
		: public JsObject
	{
	public:
		DrawableMiterLimitConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
