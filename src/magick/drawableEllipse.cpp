#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableEllipse.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableEllipse::DrawableEllipse(double originX_, double originY_, double radiusX_, double radiusY_, double arcStart_, double arcEnd_)
		: DrawableBase("[object DrawableEllipse]")
		, _impl(new ::Magick::DrawableEllipse(originX_, originY_, radiusX_, radiusY_, arcStart_, arcEnd_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableEllipse::DrawableEllipse(const DrawableEllipse &from)
		: DrawableBase("[object DrawableEllipse]")
		, _impl(new ::Magick::DrawableEllipse(from.getImpl().originX(), from.getImpl().originY(), from.getImpl().radiusX(), from.getImpl().radiusY(), from.getImpl().arcStart(), from.getImpl().arcEnd()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableEllipse::~DrawableEllipse()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableEllipse::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableEllipse(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableEllipse::xetter_originX(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->originX(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->originX(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableEllipse::xetter_originY(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->originY(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->originY(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableEllipse::xetter_radiusX(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->radiusX(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->radiusX(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableEllipse::xetter_radiusY(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->radiusY(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->radiusY(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableEllipse::xetter_arcStart(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->arcStart(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->arcStart(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableEllipse::xetter_arcEnd(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->arcEnd(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->arcEnd(d);
		}

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableEllipse &DrawableEllipse::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableEllipse::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableEllipse::call_copy, 0))return false;

		if(!jsRegisterProp("originX", &DrawableEllipse::xetter_originX, false)) return false;
		if(!jsRegisterProp("originY", &DrawableEllipse::xetter_originY, false)) return false;
		if(!jsRegisterProp("radiusX", &DrawableEllipse::xetter_radiusX, false)) return false;
		if(!jsRegisterProp("radiusY", &DrawableEllipse::xetter_radiusY, false)) return false;
		if(!jsRegisterProp("arcStart", &DrawableEllipse::xetter_arcStart, false)) return false;
		if(!jsRegisterProp("arcEnd", &DrawableEllipse::xetter_arcEnd, false)) return false;


		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableEllipseConstructor::DrawableEllipseConstructor()
		: JsObject(true, "[object DrawableEllipseConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableEllipseConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableEllipse *from = jsobjFromJsv<DrawableEllipse>(argv[0]);
			if(from)
			{
				*rval = (new DrawableEllipse(*from))->thisJsval();
			}
			else
			{
				JS_ReportError(ecx()->_jsCx, "EllipseConstructor 1st arg must be a DrawableEllipse object");
				return false;
			}
		}
		else if(6 == argc)
		{
			jsdouble vals[6];
			if(!JS_ConvertArguments(ecx()->_jsCx, 6, argv, "dddddd", vals+0, vals+1, vals+2, vals+3, vals+4, vals+5)) return false;
			*rval = (new DrawableEllipse(vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "EllipseConstructor: must be called with 1 or 6 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
