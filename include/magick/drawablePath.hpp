#ifndef _ccms_magick_drawablePath_hpp_
#define _ccms_magick_drawablePath_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawablePath
		: public DrawableBase
	{
	public:
		DrawablePath(const ::Magick::VPathList &from);
		DrawablePath(const DrawablePath &from);
		~DrawablePath();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawablePath &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawablePath> _impl;

	};
	typedef JsPtr<DrawablePath> DrawablePathPtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawablePathConstructor
		: public JsObject
	{
	public:
		DrawablePathConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
