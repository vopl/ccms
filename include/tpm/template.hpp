#ifndef _ccms_template_hpp_
#define _ccms_template_hpp_

#include "tpm/templateSource.hpp"
#include "scripter/jsObject.hpp"
#include "assocSeqContainer.hpp"

namespace ccms
{
	enum ETemplateState
	{
		ets_null,
		ets_init,	//не скомпилирован
		ets_xml,	//скомпилирован как xml
		ets_text,	//скомпилирован как текст
	};

	//////////////////////////////////////////////////////////////////////////
	class Template
		: public JsObject
	{
		//упорядоченный ассоциативный контейнер вложенных сурсов
		//упорядоченность используется при print
		//ассоциативность при формировании и подмене данных
		typedef AssocNuRndContainer<std::string, TemplateSourcePtr> TResult;
		TResult _result;

		//текущее состояние шаблона
		ETemplateState _state;

		jsval _topObject;//тот объект для которого вызвана функция print

		JSObject *_clenchContainer;

	public:
		Template();
		~Template();

 		bool setTarget(jsval target);
		bool print(JSObject *obj, std::ostream &out, const TemplateEscaper &escaper);
		ETemplateState getState();

	private:
		bool targetToText(jsval target, std::string &text);
		bool preprocessXml(JSObject *xml, bool isTop = false);
		bool preprocessXmlValue(const char *s, std::string &preprocessed, ETemplateEscaperType etet);

	public:
		bool call_print(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		bool call_toString(JSObject *obj, uintN argc, jsval *argv, jsval *rval);

		bool call_compile(uintN argc, jsval *argv, jsval *rval);
		bool call_compileXml(uintN argc, jsval *argv, jsval *rval);
		bool call_compileText(uintN argc, jsval *argv, jsval *rval);

		bool call_clone(uintN argc, jsval *argv, jsval *rval);

		bool xetter_obj(jsval *vp, bool isGet);

		virtual JSBool onPropAdd(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropDel(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropSet(JSObject *obj, jsid id, jsval *vp);
		virtual JSBool onPropGet(JSObject *obj, jsid id, jsval *vp);

		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	};
	typedef JsPtr<Template> TemplatePtr;
}
#endif
