#ifndef _ccms_blockEntry_hpp_
#define _ccms_blockEntry_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class BlockEntry
		: public JsObject
	{
		//слоты
		enum ESlot
		{
			es_priority=0,
			es_content=1,
		};

	public:
		BlockEntry();
		~BlockEntry();

		bool xetter_content(jsval *vp, bool isGet);
		bool xetter_priority(jsval *vp, bool isGet);

	private:
		//////////////////////////////////////////////////////////////////////////
		//перерулить хуки стандартного объекта, чтобы добавляемые свойства 
		//не подвергались обработкам типа "удалить по окончании запроса"
		JSBool onPropAdd(JSObject *obj, jsid id, jsval *vp);
		JSBool onPropDel(JSObject *obj, jsid id, jsval *vp);
		JSBool onPropSet(JSObject *obj, jsid id, jsval *vp);
		JSBool onPropGet(JSObject *obj, jsid id, jsval *vp);

	};

	typedef JsPtr<BlockEntry> BlockEntryPtr;
}

#endif
