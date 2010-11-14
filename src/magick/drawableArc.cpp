#include "magick_pch.h"
#include "magick/drawableArc.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableArc::DrawableArc(double startX, double startY, double endX, double endY, double startDegrees, double endDegrees)
		: DrawableBase("DrawableArc")
		, _impl(new ::Magick::DrawableArc(startX, startY, endX, endY, startDegrees, endDegrees))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableArc::DrawableArc(const DrawableArc &from)
		: DrawableBase("DrawableArc")
		, _impl(new ::Magick::DrawableArc(from.getImpl().startX(), from.getImpl().startY(), from.getImpl().endX(), from.getImpl().endY(), from.getImpl().startDegrees(), from.getImpl().endDegrees()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableArc::~DrawableArc()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableArc::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableArc(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableArc::xetter_startX(jsval *vp, bool isGet)
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
	bool DrawableArc::xetter_startY(jsval *vp, bool isGet)
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
	bool DrawableArc::xetter_endX(jsval *vp, bool isGet)
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
	bool DrawableArc::xetter_endY(jsval *vp, bool isGet)
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
	bool DrawableArc::xetter_startDegrees(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->startDegrees(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->startDegrees(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableArc::xetter_endDegrees(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->endDegrees(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->endDegrees(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableArc &DrawableArc::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableArc::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableArc::call_copy, 0))return false;

		if(!jsRegisterProp("startX",		&DrawableArc::xetter_startX, false))return false;
		if(!jsRegisterProp("startY",		&DrawableArc::xetter_startY, false))return false;
		if(!jsRegisterProp("endX",			&DrawableArc::xetter_endX, false))return false;
		if(!jsRegisterProp("endY",			&DrawableArc::xetter_endY, false))return false;
		if(!jsRegisterProp("startDegrees",	&DrawableArc::xetter_startDegrees, false))return false;
		if(!jsRegisterProp("endDegrees",	&DrawableArc::xetter_endDegrees, false))return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableArcConstructor::DrawableArcConstructor()
		: JsObject(true, "DrawableArcConstructor")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableArcConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			if(!JSVAL_IS_OBJECT(argv[0]) || JSVAL_IS_NULL(argv[0]))
			{
				JS_ReportError(ecx()->_jsCx, "ArcConstructor: arg must be an object");
				return JS_FALSE;
			}
			JSObject *jso = JSVAL_TO_OBJECT(argv[0]);
			JsObject *obj = JsObject::thisObj(jso);
			if(!obj)
			{
				JS_ReportError(ecx()->_jsCx, "ArcConstructor: arg must be an core object");
				return JS_FALSE;
			}

			DrawableArc *arc = dynamic_cast<DrawableArc *>(obj);
			if(!arc)
			{
				JS_ReportError(ecx()->_jsCx, "ArcConstructor: arg must be a DrawableArc object");
				return JS_FALSE;
			}
			*rval = (new DrawableArc(*arc))->thisJsval();
		}
		else if(6 == argc)
		{
			jsdouble vals[6];
			if(!JS_ConvertArguments(ecx()->_jsCx, 6, argv, "dddddd", vals+0, vals+1, vals+2, vals+3, vals+4, vals+5)) return false;
			*rval = (new DrawableArc(vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "ArcConstructor: must be called with 1 or 6 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
