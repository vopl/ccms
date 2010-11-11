#ifndef _ccms_magick_pathCnvtHelpers_hpp_
#define _ccms_magick_pathCnvtHelpers_hpp_

#include "scripter/jsObject.hpp"
#include "scripter/helpers.hpp"

#include "magick/pathBase.hpp"


namespace ccms{namespace magick{

	//////////////////////////////////////////////////////////////////////////
	inline bool PathArcArgs_jsv(JSObject *jso, ::Magick::PathArcArgs &args)
	{
		// double radiusX
		// double radiusY
		// double xAxisRotation
		// bool largeArcFlag
		// bool sweepFlag
		// double x
		// double y
		jsdouble d;
		bool b;

		if(!jsGetProperty(jso, "radiusX", d, "PathArcArgs")) return false;
		args.radiusX(d);

		if(!jsGetProperty(jso, "radiusY", d, "PathArcArgs")) return false;
		args.radiusY(d);

		if(!jsGetProperty(jso, "xAxisRotation", d, "PathArcArgs")) return false;
		args.xAxisRotation(d);

		if(!jsGetProperty(jso, "largeArcFlag", b, "PathArcArgs")) return false;
		args.largeArcFlag(b);

		if(!jsGetProperty(jso, "sweepFlag", b, "PathArcArgs")) return false;
		args.sweepFlag(b);

		if(!jsGetProperty(jso, "x", d, "PathArcArgs")) return false;
		args.x(d);

		if(!jsGetProperty(jso, "y", d, "PathArcArgs")) return false;
		args.y(d);

		return true;
	}

	
	//////////////////////////////////////////////////////////////////////////
	inline bool PathArcArgsList_jsv(jsval jsv, ::Magick::PathArcArgsList &list)
	{
		//одна координата или массив координат
		if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
		{
			JS_ReportError(ecx()->_jsCx, "PathArcArgsList arg must be a hash or array");
			return false;
		}

		JSObject *jso = JSVAL_TO_OBJECT(jsv);
		if(JS_IsArrayObject(ecx()->_jsCx, jso))
		{
			jsuint len;
			if(!JS_GetArrayLength(ecx()->_jsCx, jso, &len)) return false;

			for(jsuint i(0); i<len; i++)
			{
				if(!JS_GetElement(ecx()->_jsCx, jso, i, &jsv)) return false;

				if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
				{
					JS_ReportError(ecx()->_jsCx, "PathArcArgsList arg %d must be a hash", i);
					return false;
				}
				JSObject *jsoElement = JSVAL_TO_OBJECT(jsv);

				::Magick::PathArcArgs args;
				if(!PathArcArgs_jsv(jsoElement, args)) return false;
				list.push_back(args);
			}

		}
		else
		{
			::Magick::PathArcArgs args;
			if(!PathArcArgs_jsv(jso, args)) return false;
			list.push_back(args);
		}

		return true;
	}





















	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	inline bool PathCurveToArgs_jsv(JSObject *jso, ::Magick::PathCurvetoArgs &args)
	{
		// double x1
		// double y1
		// double x2
		// double y2
		// double x
		// double y

		jsdouble d;

		if(!jsGetProperty(jso, "x1", d, "PathCurveToArgs")) return false;
		args.x1(d);

		if(!jsGetProperty(jso, "y1", d, "PathCurveToArgs")) return false;
		args.y1(d);

		if(!jsGetProperty(jso, "x2", d, "PathCurveToArgs")) return false;
		args.x2(d);

		if(!jsGetProperty(jso, "y2", d, "PathCurveToArgs")) return false;
		args.y2(d);

		if(!jsGetProperty(jso, "x", d, "PathCurveToArgs")) return false;
		args.x(d);

		if(!jsGetProperty(jso, "y", d, "PathCurveToArgs")) return false;
		args.y(d);

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	inline bool PathCurveToArgsList_jsv(jsval jsv, ::Magick::PathCurveToArgsList &list)
	{
		//одна координата или массив координат
		if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
		{
			JS_ReportError(ecx()->_jsCx, "PathCurveToArgsList arg must be a hash or array");
			return false;
		}

		JSObject *jso = JSVAL_TO_OBJECT(jsv);
		if(JS_IsArrayObject(ecx()->_jsCx, jso))
		{
			jsuint len;
			if(!JS_GetArrayLength(ecx()->_jsCx, jso, &len)) return false;

			for(jsuint i(0); i<len; i++)
			{
				if(!JS_GetElement(ecx()->_jsCx, jso, i, &jsv)) return false;

				if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
				{
					JS_ReportError(ecx()->_jsCx, "PathCurveToArgsList arg %d must be a hash", i);
					return false;
				}
				JSObject *jsoElement = JSVAL_TO_OBJECT(jsv);

				::Magick::PathCurvetoArgs args;
				if(!PathCurveToArgs_jsv(jsoElement, args)) return false;
				list.push_back(args);
			}

		}
		else
		{
			::Magick::PathCurvetoArgs args;
			if(!PathCurveToArgs_jsv(jso, args)) return false;
			list.push_back(args);
		}

		return true;
	}




















	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	inline bool PathQuadraticCurveToArgs_jsv(JSObject *jso, ::Magick::PathQuadraticCurvetoArgs &args)
	{
		// double x1
		// double y1
		// double x
		// double y

		jsdouble d;

		if(!jsGetProperty(jso, "x1", d, "PathQuadraticCurveToArgs")) return false;
		args.x1(d);

		if(!jsGetProperty(jso, "y1", d, "PathQuadraticCurveToArgs")) return false;
		args.y1(d);

		if(!jsGetProperty(jso, "x", d, "PathQuadraticCurveToArgs")) return false;
		args.x(d);

		if(!jsGetProperty(jso, "y", d, "PathQuadraticCurveToArgs")) return false;
		args.y(d);

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	inline bool PathQuadraticCurveToArgsList_jsv(jsval jsv, ::Magick::PathQuadraticCurvetoArgsList &list)
	{
		//одна координата или массив координат
		if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
		{
			JS_ReportError(ecx()->_jsCx, "PathQuadraticCurveToArgsList arg must be a hash or array");
			return false;
		}

		JSObject *jso = JSVAL_TO_OBJECT(jsv);
		if(JS_IsArrayObject(ecx()->_jsCx, jso))
		{
			jsuint len;
			if(!JS_GetArrayLength(ecx()->_jsCx, jso, &len)) return false;

			for(jsuint i(0); i<len; i++)
			{
				if(!JS_GetElement(ecx()->_jsCx, jso, i, &jsv)) return false;

				if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
				{
					JS_ReportError(ecx()->_jsCx, "PathQuadraticCurveToArgsList arg %d must be a hash", i);
					return false;
				}
				JSObject *jsoElement = JSVAL_TO_OBJECT(jsv);

				::Magick::PathQuadraticCurvetoArgs args;
				if(!PathQuadraticCurveToArgs_jsv(jsoElement, args)) return false;
				list.push_back(args);
			}

		}
		else
		{
			::Magick::PathQuadraticCurvetoArgs args;
			if(!PathQuadraticCurveToArgs_jsv(jso, args)) return false;
			list.push_back(args);
		}

		return true;
	}



























	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	inline bool Coordinate_jsv(JSObject *jso, ::Magick::Coordinate &args)
	{
		// double x
		// double y

		jsdouble d;

		if(!jsGetProperty(jso, "x", d, "Coordinate")) return false;
		args.x(d);

		if(!jsGetProperty(jso, "y", d, "Coordinate")) return false;
		args.y(d);

		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	inline bool CoordinateList_jsv(jsval jsv, ::Magick::CoordinateList &list)
	{
		//одна координата или массив координат
		if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
		{
			JS_ReportError(ecx()->_jsCx, "CoordinateList arg must be a hash or array");
			return false;
		}

		JSObject *jso = JSVAL_TO_OBJECT(jsv);
		if(JS_IsArrayObject(ecx()->_jsCx, jso))
		{
			jsuint len;
			if(!JS_GetArrayLength(ecx()->_jsCx, jso, &len)) return false;

			for(jsuint i(0); i<len; i++)
			{
				if(!JS_GetElement(ecx()->_jsCx, jso, i, &jsv)) return false;

				if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
				{
					JS_ReportError(ecx()->_jsCx, "CoordinateList arg %d must be a hash", i);
					return false;
				}
				JSObject *jsoElement = JSVAL_TO_OBJECT(jsv);

				::Magick::Coordinate args;
				if(!Coordinate_jsv(jsoElement, args)) return false;
				list.push_back(args);
			}

		}
		else
		{
			::Magick::Coordinate args;
			if(!Coordinate_jsv(jso, args)) return false;
			list.push_back(args);
		}

		return true;
	}



















	inline bool Path_jsv(JSObject *jso, ::Magick::VPath &p)
	{
		JsObject *obj = JsObject::thisObj(jso);
		if(!obj)
		{
			JS_ReportError(ecx()->_jsCx, "Path is not valid");
			return false;
		}

		PathBase *pp = dynamic_cast<PathBase *>(obj);
		if(pp)
		{
			p = ::Magick::VPath(pp->getBaseImpl());
			return true;
		}

		JS_ReportError(ecx()->_jsCx, "Path is not valid Path object");
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	inline bool PathList_jsv(jsval jsv, ::Magick::VPathList &list)
	{
		//одна координата или массив координат
		if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
		{
			JS_ReportError(ecx()->_jsCx, "PathList arg must be a hash or array");
			return false;
		}

		JSObject *jso = JSVAL_TO_OBJECT(jsv);
		if(JS_IsArrayObject(ecx()->_jsCx, jso))
		{
			jsuint len;
			if(!JS_GetArrayLength(ecx()->_jsCx, jso, &len)) return false;

			for(jsuint i(0); i<len; i++)
			{
				if(!JS_GetElement(ecx()->_jsCx, jso, i, &jsv)) return false;

				if(!JSVAL_IS_OBJECT(jsv) || JSVAL_IS_NULL(jsv))
				{
					JS_ReportError(ecx()->_jsCx, "PathList arg %d must be a Path object", i);
					return false;
				}
				JSObject *jsoElement = JSVAL_TO_OBJECT(jsv);

				::Magick::VPath p;
				if(!Path_jsv(jsoElement, p)) return false;
				list.push_back(p);
			}

		}
		else
		{
			::Magick::VPath p;
			if(!Path_jsv(jso, p)) return false;
			list.push_back(p);
		}

		return true;
	}
}}

#endif
