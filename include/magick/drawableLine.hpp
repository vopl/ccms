#ifndef _ccms_magick_drawableLine_hpp_
#define _ccms_magick_drawableLine_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableLine
		: public DrawableBase
	{
	public:
		DrawableLine(double startX, double startY, double endX, double endY);
		DrawableLine(const DrawableLine &from);
		~DrawableLine();

		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_startX(jsval *vp, bool isGet);
		bool xetter_startY(jsval *vp, bool isGet);

		bool xetter_endX(jsval *vp, bool isGet);
		bool xetter_endY(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableLine &getImpl() const;

	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableLine> _impl;

	};
	typedef JsPtr<DrawableLine> LinePtr;



	//////////////////////////////////////////////////////////////////////////
	class DrawableLineConstructor
		: public JsObject
	{
	public:
		DrawableLineConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
