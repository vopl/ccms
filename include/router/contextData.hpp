#ifndef _ccms_contextData_hpp_
#define _ccms_contextData_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	//�������� ����� ������������ �������� ���������� ������ ����� ���� ������
	class ContextData
		: public JsObject
	{
	public:
		ContextData(const void *carrier);
		~ContextData();

		//���������� �� ����������
		void instanceStopped(const char *key);

		//����� ���������
		void unregister();

		const std::string &getCarrier();

	private:
		std::string _carrier;

		virtual JSBool onPropAdd(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropDel(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropSet(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropGet(JSObject *obj, jsid id, jsval *vp);

		virtual JSBool onEnumerate();

	};

	typedef JsPtr<ContextData> ContextDataPtr;
}
#endif
