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
		ets_init,	//�� �������������
		ets_xml,	//������������� ��� xml
		ets_text,	//������������� ��� �����
	};

	//////////////////////////////////////////////////////////////////////////
	class Template
		: public JsObject
	{
		//������������� ������������� ��������� ��������� ������
		//��������������� ������������ ��� print
		//��������������� ��� ������������ � ������� ������
		typedef AssocNuRndContainer<std::string, TemplateSourcePtr> TResult;
		TResult _result;

		//������� ��������� �������
		ETemplateState _state;

		jsval _topObject;//��� ������ ��� �������� ������� ������� print

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
