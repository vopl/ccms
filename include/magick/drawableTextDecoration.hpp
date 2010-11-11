#ifndef _ccms_magick_drawableDecoration_hpp_
#define _ccms_magick_drawableDecoration_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableTextDecoration
		: public DrawableBase
	{
	public:
		DrawableTextDecoration(::Magick::DecorationType from);
		DrawableTextDecoration(const DrawableTextDecoration &from);
		~DrawableTextDecoration();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_decoration(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableTextDecoration &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableTextDecoration> _impl;

	};
	typedef JsPtr<DrawableTextDecoration> DrawableTextDecorationPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableTextDecorationConstructor
		: public JsObject
	{
	public:
		DrawableTextDecorationConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
