#include "stdafx.h"
#include "struct/filter.hpp"
#include "struct/point.hpp"
#include "router/contextData.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Filter::Filter(Point *parent, FilterInstancePtr instance)
		: JsObject(true, "Filter")
		, _parent(parent)
		, _instance(instance)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	Filter::~Filter()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void Filter::init()
	{
		if(isInited())
		{
			return;
		}

// 		{
// 			ContextDataPtr cd = mkp<ContextData>(new ContextData((void *)crc32), ROOTNAME);
// 			jsRegisterProp("cd", cd->thisJsval());
// 		}

		TMPropertyInstances properties;
		_instance->getProperties_inh(properties);
		jsRegisterProp("properties", mkProperties(properties, _properties));


		if(_parent)
		{
			jsRegisterProp("parent", _parent->thisJsval());
		}


		//setPrototype(_instance);
		(JSExceptionReporter)JS_SetPrototype(ecx()->_jsCx, thisJsobj(), _instance->thisJsobj());

		Initializable::init();

		TMProperties::Assoc::iterator iter = _properties.find("startup");
		if(_properties.end() != iter)
		{
			jsval jsv;
			(JSExceptionReporter)iter->second->call_render(0, NULL, &jsv);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Filter::deinit()
	{
		if(!isInited())
		{
			return;
		}

		TMProperties::Assoc::iterator iter = _properties.find("shutdown");
		if(_properties.end() != iter)
		{
			jsval jsv;
			(JSExceptionReporter)iter->second->call_render(0, NULL, &jsv);
		}


		//setPrototype(JsPtr());
		(JSExceptionReporter)JS_SetPrototype(ecx()->_jsCx, thisJsobj(), NULL);

		deletePermanent("parent");

		deletePermanent("processProperty");
		deletePermanent("startupProperty");
		deletePermanent("shutdownProperty");

		mapDeinit(_properties);

// 		getPropertyPtr<ContextData>("cd")->unregister();
// 		deletePermanent("cd");
		Initializable::deinit();
	}

	//////////////////////////////////////////////////////////////////////////
	EFilterInstanceKind Filter::getKind()
	{
		return _instance->getKind();
	}


	//////////////////////////////////////////////////////////////////////////
	bool Filter::process(EFilterInstanceProcessResult &fpr)
	{
		fpr = efiprPartial;

		TMProperties::Assoc::iterator iter = _properties.assoc.find("process");
		if(_properties.assoc.end() != iter)
		{
			PropertyPtr process = iter->second;
			EFilterInstanceKind kind = _instance->getKind();
			const char *kindType;
			bool kindStage;
			switch(kind)
			{
			default:
			case efikUnknown:
				kindType = "unknown";
				kindStage = true;
				break;
			case efikPreTransit:
				kindType = "transit";
				kindStage = true;
				break;
			case efikPreThis:
				kindType = "this";
				kindStage = true;
				break;
			case efikPostThis:
				kindType = "this";
				kindStage = false;
				break;
			case efikPostTransit:
				kindType = "transit";
				kindStage = false;
				break;
			}
			jsval argv[2] = {kindStage?JSVAL_TRUE:JSVAL_FALSE, STRING_TO_JSVAL(JS_InternString(ecx()->_jsCx, kindType))};
			jsval res;
			if(!process->call_render(2, argv, &res)) return false;

			if(JSVAL_IS_STRING(res))
			{
				char *str;
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &res, "s", &str)) return false;
				if(!strcmp(str, "complete"))
				{
					fpr = efiprComplete;
				}
			}
		}

		return true;
	}

}
