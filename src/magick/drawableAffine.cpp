#include "magick_pch.h"
#include "magick/drawableAffine.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableAffine::DrawableAffine(double sx, double sy, double rx, double ry, double tx, double ty)
		: DrawableBase("[object DrawableAffine]")
		, _impl(new ::Magick::DrawableAffine(sx, sy, rx, ry, tx, ty))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableAffine::DrawableAffine(const DrawableAffine &from)
		: DrawableBase("[object DrawableAffine]")
		, _impl(new ::Magick::DrawableAffine(from.getImpl().sx(), from.getImpl().sy(), from.getImpl().rx(), from.getImpl().ry(), from.getImpl().tx(), from.getImpl().ty()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableAffine::DrawableAffine()
		: DrawableBase("[object DrawableAffine]")
		, _impl(new ::Magick::DrawableAffine)
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableAffine::~DrawableAffine()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableAffine::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableAffine(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableAffine::xetter_sx(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->sx(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->sx(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableAffine::xetter_sy(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->sy(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->sy(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableAffine::xetter_tx(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->tx(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->tx(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableAffine::xetter_ty(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->ty(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->ty(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableAffine::xetter_rx(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->rx(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->rx(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableAffine::xetter_ry(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			if(!JS_NewDoubleValue(ecx()->_jsCx, _impl->ry(), vp)) return false;
		}
		else
		{
			jsdouble d;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "d", &d)) return false;
			_impl->ry(d);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableAffine &DrawableAffine::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableAffine::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableAffine::call_copy, 0))return false;

		if(!jsRegisterProp("sx", &DrawableAffine::xetter_sx, false))return false;
		if(!jsRegisterProp("sy", &DrawableAffine::xetter_sy, false))return false;
		if(!jsRegisterProp("rx", &DrawableAffine::xetter_rx, false))return false;
		if(!jsRegisterProp("ry", &DrawableAffine::xetter_ry, false))return false;
		if(!jsRegisterProp("tx", &DrawableAffine::xetter_tx, false))return false;
		if(!jsRegisterProp("ty", &DrawableAffine::xetter_ty, false))return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableAffineConstructor::DrawableAffineConstructor()
		: JsObject(true, "[object DrawableAffineConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableAffineConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(0 == argc)
		{
			*rval = (new DrawableAffine)->thisJsval();
		}
		else if(1 == argc)
		{
			if(!JSVAL_IS_OBJECT(argv[0]) || JSVAL_IS_NULL(argv[0]))
			{
				JS_ReportError(ecx()->_jsCx, "AffineConstructor: arg must be an object");
				return JS_FALSE;
			}
			JSObject *jso = JSVAL_TO_OBJECT(argv[0]);
			JsObject *obj = JsObject::thisObj(jso);
			if(!obj)
			{
				JS_ReportError(ecx()->_jsCx, "AffineConstructor: arg must be an core object");
				return JS_FALSE;
			}

			DrawableAffine *affine = dynamic_cast<DrawableAffine *>(obj);
			if(!affine)
			{
				JS_ReportError(ecx()->_jsCx, "AffineConstructor: arg must be a DrawableAffine object");
				return JS_FALSE;
			}
			*rval = (new DrawableAffine(*affine))->thisJsval();
		}
		else if(6 == argc)
		{
			jsdouble vals[6];
			if(!JS_ConvertArguments(ecx()->_jsCx, 6, argv, "dddddd", vals+0, vals+1, vals+2, vals+3, vals+4, vals+5)) return false;
			*rval = (new DrawableAffine(vals[0], vals[1], vals[2], vals[3], vals[4], vals[5]))->thisJsval();
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "AffineConstructor: must be called with 1 or 6 args");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
