#ifndef _ccms_magick_affine_hpp_
#define _ccms_magick_affine_hpp_

#include "scripter/jsObject.hpp"
#include "magick/core.h"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class Affine
		: public JsObject
	{
	public:
		Affine(double sx, double sy, double rx, double ry, double tx, double ty);
		Affine(const Affine &from);
		Affine();
		~Affine();

		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_sx(jsval *vp, bool isGet);
		bool xetter_sy(jsval *vp, bool isGet);

		bool xetter_tx(jsval *vp, bool isGet);
		bool xetter_ty(jsval *vp, bool isGet);

		bool xetter_rx(jsval *vp, bool isGet);
		bool xetter_ry(jsval *vp, bool isGet);

		const ::Magick::DrawableAffine &getImpl() const;

	private:
		bool registerJs();
		boost::shared_ptr<::Magick::DrawableAffine> _impl;

	};
	typedef JsPtr<Affine> AffinePtr;



	//////////////////////////////////////////////////////////////////////////
	class AffineConstructor
		: public JsObject
	{
	public:
		AffineConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	};

}}

#endif
