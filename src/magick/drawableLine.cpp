#include "magick_pch.h"
#include "magick/drawableLine.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableLine::DrawableLine(double startX, double startY, double endX, double endY)
		: DrawableBase("[object DrawableLine]")
		, _impl(new ::Magick::DrawableLine(startX, startY, endX, endY))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableLine::DrawableLine(const DrawableLine &from)
		: DrawableBase("[object DrawableLine]")
		, _impl(new ::Magick::DrawableLine(from.getImpl().startX(), from.getImpl().startY(), from.getImpl().endX(), from.getImpl().endY()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableLine::~DrawableLine()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableLine::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableLine(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableLine::xetter_startX(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->startX(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->startX(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableLine::xetter_startY(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->startY(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->startY(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableLine::xetter_endX(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->endX(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->endX(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableLine::xetter_endY(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->endY(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->endY(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableLine &DrawableLine::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableLine::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableLine::call_copy, 0))return false;

		if(!jsRegisterProp("startX",		&DrawableLine::xetter_startX, false))return false;
		if(!jsRegisterProp("startY",		&DrawableLine::xetter_startY, false))return false;
		if(!jsRegisterProp("endX",			&DrawableLine::xetter_endX, false))return false;
		if(!jsRegisterProp("endY",			&DrawableLine::xetter_endY, false))return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableLineConstructor::DrawableLineConstructor()
		: JsObject(true, "[object DrawableLineConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableLineConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			if(!JSVAL_IS_OBJECT(argv[0]) || JSVAL_IS_NULL(argv[0]))
			{
				JS_ReportError(ecx()->_jsCx, "LineConstructor: arg must be an object");
				return JS_FALSE;
			}
			JSObject *jso = JSVAL_TO_OBJECT(argv[0]);
			JsObject *obj = JsObject::thisObj(jso);
			if(!obj)
			{
				JS_ReportError(ecx()->_jsCx, "LineConstructor: arg must be an core object");
				return JS_FALSE;
			}

			DrawableLine *arc = dynamic_cast<DrawableLine *>(obj);
			if(!arc)
			{
				JS_ReportError(ecx()->_jsCx, "LineConstructor: arg must be a DrawableLine object");
				return JS_FALSE;
			}
			*rval = (new DrawableLine(*arc))->thisJsval();
		}
		else if(4 == argc)
		{
			jsdouble vals[4];
			if(!JS_ConvertArguments(ecx()->_jsCx, 6, argv, "dddd", vals+0, vals+1, vals+2, vals+3)) return false;
			*rval = (new DrawableLine(vals[0], vals[1], vals[2], vals[3]))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "LineConstructor: must be called with 1 or 4 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
