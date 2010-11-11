#include "magick_pch.h"
#include "magick/macroHelpers.hpp"
#include "magick/drawableDashArray.hpp"

namespace ccms {namespace magick{

	//////////////////////////////////////////////////////////////////////////
	DrawableDashArray::DrawableDashArray(const double* dasharray_)
		: DrawableBase("[object DrawableDashArray]")
		, _impl(new ::Magick::DrawableDashArray(dasharray_))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableDashArray::DrawableDashArray(const DrawableDashArray &from)
		: DrawableBase("[object DrawableDashArray]")
		, _impl(new ::Magick::DrawableDashArray(from.getImpl()))
	{
		(JSExceptionReporter)registerJs();
	}

	//////////////////////////////////////////////////////////////////////////
	DrawableDashArray::~DrawableDashArray()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableDashArray::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		*rval = (new DrawableDashArray(*this))->thisJsval();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableDashArray::xetter_dasharray(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			const double *dvals = _impl->dasharray();
			std::vector<jsval> jsvals;
			while(*dvals)
			{
				jsval jsv;
				if(!JS_NewDoubleValue(ecx()->_jsCx, *dvals, &jsv)) return false;

				jsvals.push_back(jsv);
				dvals++;
			}

			if(jsvals.size())
			{
				JSObject *obj = JS_NewArrayObject(ecx()->_jsCx, jsvals.size(), &jsvals[0]);
				*vp = OBJECT_TO_JSVAL(obj);
			}
			else
			{
				JSObject *obj = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);
				*vp = OBJECT_TO_JSVAL(obj);
			}
		}
		else
		{
			if(!JSVAL_IS_OBJECT(*vp) || JSVAL_IS_NULL(*vp))
			{
				JS_ReportError(ecx()->_jsCx, "DrawableDashArray.dasharray must be a Array of numbers");
				return false;
			}
			JSObject *arrobj = JSVAL_TO_OBJECT(*vp);
			if(!JS_IsArrayObject(ecx()->_jsCx, arrobj))
			{
				JS_ReportError(ecx()->_jsCx, "DrawableDashArray.dasharray must be a Array of numbers");
				return false;
			}

			jsuint len;
			JS_GetArrayLength(ecx()->_jsCx, arrobj, &len);

			std::vector<jsdouble> dvals(len+1);
			for(jsuint i(0); i<len; i++)
			{
				jsval jsv;
				if(!JS_GetElement(ecx()->_jsCx, arrobj, i, &jsv)) return false;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "d", &dvals[i])) return false;
			}
			_impl->dasharray(&dvals[0]);
		}

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	const ::Magick::DrawableDashArray &DrawableDashArray::getImpl() const
	{
		return *_impl;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DrawableDashArray::registerJs()
	{
		if(!jsRegisterMeth("copy", &DrawableDashArray::call_copy, 0))return false;

		if(!jsRegisterProp("dasharray", &DrawableDashArray::xetter_dasharray, false)) return false;

		return true;
	}









	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	DrawableDashArrayConstructor::DrawableDashArrayConstructor()
		: JsObject(true, "[object DrawableDashArrayConstructor]")
	{
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool DrawableDashArrayConstructor::onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(1 == argc)
		{
			DrawableDashArray *from = jsobjFromJsv<DrawableDashArray>(argv[0]);
			if(from)
			{
				*rval = (new DrawableDashArray(*from))->thisJsval();
			}
			else
			{
				if(!JSVAL_IS_OBJECT(argv[0]) || JSVAL_IS_NULL(argv[0]))
				{
					JS_ReportError(ecx()->_jsCx, "DashArrayConstructor 1st arg must be a Array of numbers");
					return false;
				}
				JSObject *arrobj = JSVAL_TO_OBJECT(argv[0]);
				if(!JS_IsArrayObject(ecx()->_jsCx, arrobj))
				{
					JS_ReportError(ecx()->_jsCx, "DashArrayConstructor 1st arg must be a Array of numbers");
					return false;
				}

				jsuint len;
				JS_GetArrayLength(ecx()->_jsCx, arrobj, &len);

				std::vector<jsdouble> dvals(len+1);
				for(jsuint i(0); i<len; i++)
				{
					jsval jsv;
					if(!JS_GetElement(ecx()->_jsCx, arrobj, i, &jsv)) return false;
					if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "d", &dvals[i])) return false;
				}
				*rval = (new DrawableDashArray(&dvals[0]))->thisJsval();
			}
		}
		else
		{
			JS_ReportError(ecx()->_jsCx, "DashArrayConstructor: must be called with 1 arg");
			return JS_FALSE;
		}

		return JS_TRUE;
	}


}}
