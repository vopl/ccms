#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableMatte.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableMatte::DrawableMatte(double x_, double y_, ::Magick::PaintMethod paintMethod_)
		: DrawableBase("[object DrawableMatte]")
		, _impl(new ::Magick::DrawableMatte(x_, y_, paintMethod_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableMatte::DrawableMatte(const DrawableMatte &from)
		: DrawableBase("[object DrawableMatte]")
		, _impl(new ::Magick::DrawableMatte(from.getImpl().x(), from.getImpl().y(), from.getImpl().paintMethod()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableMatte::~DrawableMatte()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableMatte::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableMatte(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableMatte::xetter_x(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->x(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->x(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableMatte::xetter_y(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->y(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->y(d);
		}

		return true;
	}

	bool DrawableMatte::xetter_paintMethod(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = INT_TO_JSVAL(_impl->paintMethod());
		}
		else
		{
			uint32 v;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "u", &v)) return false;
			_impl->paintMethod((::Magick::PaintMethod)v);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableMatte &DrawableMatte::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableMatte::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableMatte::call_copy, 0))return false;

		if(!jsRegisterProp("x", &DrawableMatte::xetter_x, false)) return false;
		if(!jsRegisterProp("y", &DrawableMatte::xetter_y, false)) return false;
		if(!jsRegisterProp("paintMethod", &DrawableMatte::xetter_paintMethod, false)) return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableMatteConstructor::DrawableMatteConstructor()
		: JsObject(true, "[object DrawableMatteConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableMatteConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableMatte *from = jsobjFromJsv<DrawableMatte>(argv[0]);
			if(from)
			{
				*rval = (new DrawableMatte(*from))->thisJsval();
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "MatteConstructor 1st arg must be a DrawableMatte object");
				return false;
			}
		}
		else if(3 == argc)
		{
			jsdouble x, y;
			uint32 paintMethod;

			if(!JS_ConvertArguments(ecx()->_jsCx, 3, argv, "ddu", &x, &y, &paintMethod)) return false;
			*rval = (new DrawableMatte(x, y, (::Magick::PaintMethod)paintMethod))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "MatteConstructor: must be called with 1 or 3 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
