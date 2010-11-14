#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableGravity.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableGravity::DrawableGravity(::Magick::GravityType from)
		: DrawableBase("DrawableGravity")
		, _impl(new ::Magick::DrawableGravity(from))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableGravity::DrawableGravity(const DrawableGravity &from)
		: DrawableBase("DrawableGravity")
		, _impl(new ::Magick::DrawableGravity(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableGravity::~DrawableGravity()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableGravity::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableGravity(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableGravity::xetter_gravity(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->gravity());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->gravity((::Magick::GravityType)v);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableGravity &DrawableGravity::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableGravity::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableGravity::call_copy, 0))return false;

		if(!jsRegisterProp("gravity", &DrawableGravity::xetter_gravity, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableGravityConstructor::DrawableGravityConstructor()
		: JsObject(true, "DrawableGravityConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableGravityConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableGravity *from = jsobjFromJsv<DrawableGravity>(argv[0]);
			if(from)
			{
				*rval = (new DrawableGravity(*from))->thisJsval();
			}
			else
			{
				uint32 v;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "u", &v)) return false;
				*rval = (new DrawableGravity((::Magick::GravityType)v))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "GravityConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
