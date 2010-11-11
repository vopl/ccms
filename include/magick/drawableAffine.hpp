#ifndef _ccms_magick_drawableAffine_hpp_
#define _ccms_magick_drawableAffine_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableAffine
		: public DrawableBase
	{
	public:
		DrawableAffine(double sx, double sy, double rx, double ry, double tx, double ty);
		DrawableAffine(const DrawableAffine &from);
		DrawableAffine();
		~DrawableAffine();

		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_sx(jsval *vp, bool isGet);
		bool xetter_sy(jsval *vp, bool isGet);

		bool xetter_tx(jsval *vp, bool isGet);
		bool xetter_ty(jsval *vp, bool isGet);

		bool xetter_rx(jsval *vp, bool isGet);
		bool xetter_ry(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableAffine &getImpl() const;

	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableAffine> _impl;

	};
	typedef JsPtr<DrawableAffine> DrawableAffinePtr;



	//////////////////////////////////////////////////////////////////////////
	class DrawableAffineConstructor
		: public JsObject
	{
	public:
		DrawableAffineConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
