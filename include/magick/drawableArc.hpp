#ifndef _ccms_magick_drawableArc_hpp_
#define _ccms_magick_drawableArc_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableArc
		: public DrawableBase
	{
	public:
		DrawableArc(double startX, double startY, double endX, double endY, double startDegrees, double endDegrees);
		DrawableArc(const DrawableArc &from);
		~DrawableArc();

		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_startX(jsval *vp, bool isGet);
		bool xetter_startY(jsval *vp, bool isGet);

		bool xetter_endX(jsval *vp, bool isGet);
		bool xetter_endY(jsval *vp, bool isGet);

		bool xetter_startDegrees(jsval *vp, bool isGet);
		bool xetter_endDegrees(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableArc &getImpl() const;

	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableArc> _impl;

	};
	typedef JsPtr<DrawableArc> ArcPtr;



	//////////////////////////////////////////////////////////////////////////
	class DrawableArcConstructor
		: public JsObject
	{
	public:
		DrawableArcConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
