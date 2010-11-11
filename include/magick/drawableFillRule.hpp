#ifndef _ccms_magick_drawableFillRule_hpp_
#define _ccms_magick_drawableFillRule_hpp_

#include "magick/drawableBase.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class DrawableFillRule
		: public DrawableBase
	{
	public:
		DrawableFillRule(::Magick::FillRule from);
		DrawableFillRule(const DrawableFillRule &from);
		~DrawableFillRule();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_fillRule(jsval *vp, bool isGet);

		virtual ::Magick::Drawable getBaseImpl(){return ::Magick::Drawable(*_impl);}
		const ::Magick::DrawableFillRule &getImpl() const;
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::DrawableFillRule> _impl;

	};
	typedef JsPtr<DrawableFillRule> DrawableFillRulePtr;


	//////////////////////////////////////////////////////////////////////////
	class DrawableFillRuleConstructor
		: public JsObject
	{
	public:
		DrawableFillRuleConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
