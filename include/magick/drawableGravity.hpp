#ifndef _ccms_magick_drawableGravity_hpp_
#define _ccms_magick_drawableGravity_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableGravity
		: public DrawableBase
	{
	public:
		DrawableGravity(::Magick::GravityType from);
		DrawableGravity(const DrawableGravity &from);
		~DrawableGravity();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_gravity(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableGravity &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableGravity> _impl;

	};
	typedef JsPtr<DrawableGravity> DrawableGravityPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableGravityConstructor
		: public JsObject
	{
	public:
		DrawableGravityConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
