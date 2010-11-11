#ifndef _ccms_magick_drawableRotation_hpp_
#define _ccms_magick_drawableRotation_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableRotation
		: public DrawableBase
	{
	public:
		DrawableRotation(double angle_);
		DrawableRotation(const DrawableRotation &from);
		~DrawableRotation();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_angle(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableRotation &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableRotation> _impl;

	};
	typedef JsPtr<DrawableRotation> DrawableRotationPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableRotationConstructor
		: public JsObject
	{
	public:
		DrawableRotationConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
