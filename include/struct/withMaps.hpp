#ifndef _ccms_withMaps_hpp_
#define _ccms_withMaps_hpp_

#include "exceptions.hpp"

namespace ccms
{
	typedef std::set<std::string>				TSNames;

	template <class Target>
	class WithMaps
		: public JsObject
		, public PropertyInstanceParent
	{

	public:
		WithMaps(bool doJsCreate = true, const char *strVal=NULL)
			: JsObject(doJsCreate, strVal)
		{
		}

	protected:
		template <class TMap>
		void mapInit(TMap &m, JSObject *props4);

		template <class TMap>
		void mapDeinit(TMap &m);

	protected:
		//////////////////////////////////////////////////////////////////////////
		template<class TMap>
		bool setMapElem(TMap &map, const char *name, const typename TMap::Value &child);

		bool hideMapElem(TSNames &names, const char *name, bool hide);

		template<class TMap>
		bool renameMapElem(TMap &map, const char *oldName, const char *newName);

		template<class TMap>
		bool moveMapElem(TMap &map, const char *name, int pos);

	protected:
		//setChild(name, point)
		template <class TMap, TMap Target::*m>
		bool call_setMapElem(uintN argc, jsval *argv, jsval *rval)
		{
			TMap &map = ((Target*)this)->*m;

			if(argc < 2)
			{
				JS_ReportError(ecx()->_jsCx, "[WithMaps.setMapElement must be called with 2 arguments]");
				return false;
			}
			char *name;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &name))return false;

			if(JSVAL_IS_NULL(argv[1]) || JSVAL_IS_VOID(argv[1]))
			{
				*rval = setMapElem(map, name, typename TMap::Value())?JSVAL_TRUE:JSVAL_FALSE;
				return true;
			}

			JSObject *obj=NULL;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv+1, "o", &obj))return false;

			typename TMap::Value child = mkp<typename TMap::Value::value_type>(obj, ROOTNAME);
			if(!child)
			{
				JS_ReportError(ecx()->_jsCx, "[WithMaps.setMapElement second arg must be an %s]", typeid(typename TMap::Value::value_type).name());
				return false;
			}

			*rval = setMapElem(map, name, child)?JSVAL_TRUE:JSVAL_FALSE;

			return true;
		}

		//hideChild(name, hide)
		template <TSNames Target::*m>
		bool call_hideMapElem(uintN argc, jsval *argv, jsval *rval)
		{
			TSNames &map = ((Target*)this)->*m;
			if(argc < 2)
			{
				JS_ReportError(ecx()->_jsCx, "[WithMaps.hideMapElement must be called with 2 arguments]");
				return false;
			}
			char *name;
			JSBool b;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "sb", &name, &b))return false;

			*rval = hideMapElem(map, name, b?true:false)?JSVAL_TRUE:JSVAL_FALSE;
			return true;
		}

		//renameChild(oldName, newName)
		template <class TMap, TMap Target::*m>
		bool call_renameMapElem(uintN argc, jsval *argv, jsval *rval)
		{
			TMap &map = ((Target*)this)->*m;
			if(argc < 2)
			{
				JS_ReportError(ecx()->_jsCx, "[WithMaps.renameMapElement must be called with 2 arguments]");
				return false;
			}
			char *oldName;
			char *newName;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "ss", &oldName, &newName))return false;

			try
			{
				*rval = renameMapElem(map, oldName, newName)?JSVAL_TRUE:JSVAL_FALSE;
			}
			catch(ccms::exception_badPathPart &e)
			{
				JS_ReportError(ecx()->_jsCx, "[WithMaps.renameMapElement: %s]", e.what());
				return false;
			}
			return true;
		}

		//moveChild(name, pos)//+-integer
		template <class TMap, TMap Target::*m>
		bool call_moveMapElem(uintN argc, jsval *argv, jsval *rval)
		{
			TMap &map = ((Target*)this)->*m;
			if(argc < 2)
			{
				JS_ReportError(ecx()->_jsCx, "[WithMaps.moveMapElement must be called with 2 arguments]");
				return false;
			}
			char *name;
			int32 pos;
			if(!JS_ConvertArguments(ecx()->_jsCx, 2, argv, "si", &name, &pos))return false;

			*rval = moveMapElem(map, name, pos)?JSVAL_TRUE:JSVAL_FALSE;
			return true;
		}


	};
}
#endif
