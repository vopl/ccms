#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableViewbox.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableViewbox::DrawableViewbox(uint32 x1_, uint32 y1_, uint32 x2_, uint32 y2_)
		: DrawableBase("[object DrawableViewbox]")
		, _impl(new ::Magick::DrawableViewbox(x1_, y1_, x2_, y2_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableViewbox::DrawableViewbox(const DrawableViewbox &from)
		: DrawableBase("[object DrawableViewbox]")
		, _impl(new ::Magick::DrawableViewbox(from.getImpl().x1(), from.getImpl().y1(), from.getImpl().x2(), from.getImpl().y2()))
	{

	}

	//////////////////////////////////////////////////////////////////////////
	DrawableViewbox::~DrawableViewbox()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableViewbox::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableViewbox(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableViewbox::xetter_x1(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->x1(), vp)) return false;
		}
		else
		{
			uint32 d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &d)) return false;
			_impl->x1(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableViewbox::xetter_y1(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->y1(), vp)) return false;
		}
		else
		{
			uint32 d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &d)) return false;
			_impl->y1(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableViewbox::xetter_x2(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->x2(), vp)) return false;
		}
		else
		{
			uint32 d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &d)) return false;
			_impl->x2(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableViewbox::xetter_y2(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->y2(), vp)) return false;
		}
		else
		{
			uint32 d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &d)) return false;
			_impl->y2(d);
		}

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableViewbox &DrawableViewbox::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableViewbox::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableViewbox::call_copy, 0))return false;

		if(!jsRegisterProp("x1", &DrawableViewbox::xetter_x1, false)) return false;
		if(!jsRegisterProp("y1", &DrawableViewbox::xetter_y1, false)) return false;
		if(!jsRegisterProp("x2", &DrawableViewbox::xetter_x2, false)) return false;
		if(!jsRegisterProp("y2", &DrawableViewbox::xetter_y2, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableViewboxConstructor::DrawableViewboxConstructor()
		: JsObject(true, "[object DrawableViewboxConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableViewboxConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableViewbox *circle = jsobjFromJsv<DrawableViewbox>(argv[0]);
			if(!circle)
			{
				JS_ReportError(ecx()->_jsCx, "ViewboxConstructor: arg must be a DrawableViewbox object");
				return JS_FALSE;
			}
			*rval = (new DrawableViewbox(*circle))->thisJsval();
		}
		else if(4 == argc)
		{
			uint32 x1_, y1_, x2_, y2_;
			if(!JS_ConvertArguments(ecx()->_jsCx, 4, argv, "uuuu", &x1_, &y1_, &x2_, &y2_)) return false;

			*rval = (new DrawableViewbox(x1_, y1_, x2_, y2_))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "ViewboxConstructor: must be called with 1 or 4 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
