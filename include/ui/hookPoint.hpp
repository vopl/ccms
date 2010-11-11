#ifndef _ccms_hookPoint_hpp_
#define _ccms_hookPoint_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class Ui;
	typedef JsPtr<Ui> UiPtr;

	//////////////////////////////////////////////////////////////////////////
	typedef std::string THookId;

	//////////////////////////////////////////////////////////////////////////
	class HookPoint
		: public JsObject
	{
	public:
		HookPoint(Ui *ui, THookId id, jsval data);
		~HookPoint();

	private:
		bool call_print(JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	private:
		Ui	*_ui;
		THookId	_id;
	};

	typedef JsPtr<HookPoint> HookPointPtr;
}

#endif
