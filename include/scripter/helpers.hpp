#ifndef _ccms_helpers_hpp_
#define _ccms_helpers_hpp_

namespace ccms
{













	//////////////////////////////////////////////////////////////////////////
	/*
	*   b      JSBool          Boolean
	*   c      uint16/jschar   ECMA uint16, Unicode char
	*   i      int32           ECMA int32
	*   u      uint32          ECMA uint32
	*   j      int32           Rounded int32 (coordinate)
	*   d      jsdouble        IEEE double
	*   I      jsdouble        Integral IEEE double
	*   s      char *          C string
	*   S      JSString *      Unicode string, accessed by a JSString pointer
	*   W      jschar *        Unicode character vector, 0-terminated (W for wide)
	*   o      JSObject *      Object reference
	*   f      JSFunction *    Function private
	*   v      jsval           Argument value (no conversion)
	*/

	template <class Target> 
	struct jsGetProperty_formatHelper{typedef void		jsRawType;	static const char *format(){return NULL;}};
	//template <> struct jsGetProperty_formatHelper<JSBool	>{typedef JSBool	jsRawType;	static const char *format(){return "b";}};
	template <> struct jsGetProperty_formatHelper<bool		>{typedef JSBool	jsRawType;	static const char *format(){return "b";}};
	template <> struct jsGetProperty_formatHelper<uint16	>{typedef uint16	jsRawType;	static const char *format(){return "c";}};
	template <> struct jsGetProperty_formatHelper<int32		>{typedef int32		jsRawType;	static const char *format(){return "i";}};
	template <> struct jsGetProperty_formatHelper<uint32	>{typedef uint32	jsRawType;	static const char *format(){return "u";}};
	template <> struct jsGetProperty_formatHelper<double	>{typedef jsdouble	jsRawType;	static const char *format(){return "d";}};


	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	inline bool jsGetProperty(JSObject *jso, const char *name, Target &ret, const char *objName4AbsentErr = NULL)
	{
		JSBool b;
		if(!JS_HasProperty(ecx()->_jsCx, jso, name, &b)) return false;
		if(!b)
		{
			if(objName4AbsentErr)
			{
				JS_ReportError(ecx()->_jsCx, "%s must contain attribute %s", objName4AbsentErr, name);
				return false;
			}
			ret = 0;
			return true;
		}

		jsval jsv;
		if(!JS_GetProperty(ecx()->_jsCx, jso, name, &jsv)) return false;
		typename jsGetProperty_formatHelper<Target>::jsRawType holder;

		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, jsGetProperty_formatHelper<Target>::format(), &holder)) return false;
		ret = holder;

		return true;
	}

}

#endif
