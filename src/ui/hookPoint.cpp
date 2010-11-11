#include "stdafx.h"
#include "ui/hookPoint.hpp"
#include "ui/ui.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	static int hc=0;
	HookPoint::HookPoint(Ui *ui, THookId id, jsval data)
		: JsObject(true, "[object HookPoint]")
		, _ui(ui)
		, _id(id)
	{
		hc++;
		//jsRegisterProp("ui", _ui->thisJsval(), true);

		jsRegisterProp("id", id, true);
		jsRegisterProp("data", data, true);

		jsRegisterMeth("print", &HookPoint::call_print, 0);
	}

	//////////////////////////////////////////////////////////////////////////
	HookPoint::~HookPoint()
	{
		hc--;

	}

	//////////////////////////////////////////////////////////////////////////
	bool HookPoint::call_print(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		jsval jsvThis = OBJECT_TO_JSVAL(obj);
		if(!_ui->call_hookInvoke(1, &jsvThis, rval))
		{
			if(!JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportError(ecx()->_jsCx, "HookPoint.print failed to call ui.hookInvoke");
			}
			*rval = JSVAL_VOID;
			return false;
		}
		return true;
	}

}
