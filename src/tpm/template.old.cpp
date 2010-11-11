#include "stdafx.h"
#include "tpm/template.hpp"
#include "utils/ncvt.h"

//можно строить префиксы и суфиксы динамически, тогда они будут короче, но их надо будет гдето хранить а пам€ти жалко
#define MARK "mJoivl_8jls4ls2fk_9HNqmia8sKPiMsg2bcmr"

#define MARK_PREFIX_TAGNAME			MARK "_tn_"
#define MARK_PREFIX_ATTRNAME		MARK "_an_"
#define MARK_PREFIX_ATTRVALUE		MARK "_av_"
#define MARK_PREFIX_XML				MARK "_x_"

#define MARK_PREFIX					MARK "_p_"
#define MARK_SUFFIX					MARK "_s_"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	Template::Template()
		: JsObject(true, "[object Template]")
		, _compiledFromXml(false)
	{
		jsRegisterMeth("print", &Template::call_print, 0);
		jsRegisterMeth("toString", &Template::call_toString, 0);

		jsRegisterProp("target", &Template::xetter_target, false);
		jsRegisterProp("xml", &Template::xetter_xml, false);
		jsRegisterProp("text", &Template::xetter_text, false);

		_dataArray = JS_NewArrayObject(ecx()->_jsCx, 0, NULL);
		jsRegisterProp("dataArray", OBJECT_TO_JSVAL(_dataArray), true);
	}

	//////////////////////////////////////////////////////////////////////////
	Template::~Template()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::generateMarks(uintN argc, jsval *argv, std::string &marks)
	{
		if(!_dataArray)
		{
			JS_ReportError(ecx()->_jsCx, "[Template.generateMarks already compiled]");
			return false;
		}
		jsuint length;
		if(!JS_GetArrayLength(ecx()->_jsCx, _dataArray, &length)) return false;
		jsval rval;
		if(!JS_CallFunctionName(ecx()->_jsCx, _dataArray, "push", argc, argv, &rval)) return false;

		for(uintN i(0); i<argc; i++)
		{
			marks += MARK_PREFIX;
			marks += _ntoa(length+i);//индекс добавленного сурса
			marks += MARK_SUFFIX;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::setTarget(jsval target)
	{
		if(!_dataArray)
		{
			JS_ReportError(ecx()->_jsCx, "[Template.setTarget already compiled]");
			return false;
		}

		std::string text;
		if(!targetToText(target, text))
		{
			return false;
		}
		//вы€вить метки, сформировать тексты и данные
		using namespace boost::xpressive;
		static const cregex re =
			repeat<0,1>(
			(s1=as_xpr(MARK_PREFIX_TAGNAME)) |
			(s2=as_xpr(MARK_PREFIX_ATTRNAME)) |
			(s3=as_xpr(MARK_PREFIX_ATTRVALUE)) |
			(s4=as_xpr(MARK_PREFIX_XML))) >>
			(as_xpr(MARK_PREFIX)>> 	(s5=(+_d))>>	as_xpr(MARK_SUFFIX));
		boost::xpressive::cmatch what;


		typedef std::set<TemplateSourcePtr, TemplateSource::PtrPred> TSSources;
		TSSources forDoubles;
		std::vector<TemplateSourcePtr> result;
		const char *iter = text.data();
		const char *end = text.data() + text.size();
		while(boost::xpressive::regex_search(iter, end, what, re))
		{
			const char *sbeg = iter+what.position();
			const char *send = sbeg+what.length();

			if(sbeg != iter)
			{
				TemplateSourcePtr t(new TemplateSource);
				t->setText(iter, sbeg-iter);

				TSSources::_Pairib pb = forDoubles.insert(t);
				result.push_back(*pb.first);
			}

			size_t mark = _atost(what[5].str().data());
			jsval jsv = JSVAL_VOID;
			if(!JS_GetElement(ecx()->_jsCx, _dataArray, mark, &jsv)) return false;

			TemplateSourcePtr t(new TemplateSource);
			t->setJsval(jsv);

			if(what[1].matched) t->setEtet(etetTagName);
			else if(what[2].matched) t->setEtet(etetAttrName);
			else if(what[3].matched) t->setEtet(etetAttrValue);
			else if(what[4].matched)
			{
				if(JS_TypeOfValue(ecx()->_jsCx, jsv) == JSTYPE_XML)
				{
					// already xml
					t->setEtet(etetNull);
				}
				else
				{
					t->setEtet(etetXml);
				}
			}

			TSSources::_Pairib pb = forDoubles.insert(t);
			result.push_back(*pb.first);

			iter = send;
		}

		if(end != iter)
		{
			TemplateSourcePtr t(new TemplateSource);
			t->setText(iter, end-iter);

			TSSources::_Pairib pb = forDoubles.insert(t);
			result.push_back(*pb.first);
		}

		//больше не понадобитс€
		forDoubles.clear();

		//////////////////////////////////////////////////////////////////////////
		//применить сформированные сурсы
		_sources.reserve(result.size());
		_sources.assign(result.begin(), result.end());

		//бросить исходнве данные
		deletePermanent("dataArray");
		_dataArray = NULL;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::print(std::ostream &out, const TemplateEscaper &escaper)
	{
		BOOST_FOREACH(TemplateSourcePtr &s, _sources)
		{
			if(!s->print(out, escaper))
			{
				//assert(0);
				return false;
			}
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::compiledFromXml()
	{
		return _compiledFromXml;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::targetToText(jsval target, std::string &text)
	{
		if(JSTYPE_XML == JS_TypeOfValue(ecx()->_jsCx, target))
		{
			//клонировать чтобы оригинал осталс€ неизменным
			JS_CallFunctionName(ecx()->_jsCx, JSVAL_TO_OBJECT(target), "copy", 0, NULL, &target);

			//подставить типизацию префиксами
			if(!preprocessXml(JSVAL_TO_OBJECT(target)))
			{
				return false;
			}
			//и привести к тексту
			JS_CallFunctionName(ecx()->_jsCx, JSVAL_TO_OBJECT(target), "toXMLString", 0, NULL, &target);
			_compiledFromXml = true;
		}
		else
		{
			_compiledFromXml = false;
		}
		JSString *str;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &target, "S", &str))
		{
			return false;
		}
		size_t size = JS_GetStringLength(str);
		char *data = JS_EncodeString(ecx()->_jsCx, str);
		text.assign(data, data+size);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::preprocessXml(JSObject *xml)
	{
		JSContext *cx = ecx()->_jsCx;
		jsval jsv;
		char *s;


		//тип узла
		//attribute, element, comment, processing-instruction, text
		if(!JS_CallFunctionName(cx, xml, "nodeKind", 0, NULL, &jsv)) return false;
		if(!JS_ConvertArguments(cx, 1, &jsv, "s", &s)) return false;

		if(!strcmp("text", s))
		{
			//как текст
			std::string preprocessed;

			if(!JS_CallFunctionName(cx, xml, "toXMLString", 0, NULL, &jsv)) return false;
			if(!JS_ConvertArguments(cx, 1, &jsv, "s", &s)) return false;

			if(preprocessXmlValue(s, preprocessed, etetXml))
			{
				jsv = STRING_TO_JSVAL(JS_NewStringCopyN(cx, preprocessed.data(), preprocessed.size()));

				jsval index;
				if(!JS_CallFunctionName(cx, xml, "childIndex", 0, NULL, &index)) return false;
				jsval parent;
				if(!JS_CallFunctionName(cx, xml, "parent", 0, NULL, &parent)) return false;

				jsval argv[2] = {index, jsv};
				if(!JS_CallFunctionName(cx, JSVAL_TO_OBJECT(parent), "replace", 2, argv, &jsv)) return false;
			}
			return true;
		}
		if(!strcmp("element", s))
		{
			//как элемент
			std::string preprocessed;

			//тэг
			if(!JS_CallFunctionName(cx, xml, "name", 0, NULL, &jsv)) return false;
			if(!JS_ConvertArguments(cx, 1, &jsv, "s", &s)) return false;

			if(preprocessXmlValue(s, preprocessed, etetTagName))
			{
				jsv = STRING_TO_JSVAL(JS_NewStringCopyN(cx, preprocessed.data(), preprocessed.size()));
				if(!JS_CallFunctionName(cx, xml, "setName", 1, &jsv, &jsv)) return false;
			}


			//аттрибуты
			if(!JS_CallFunctionName(cx, xml, "attributes", 0, NULL, &jsv)) return false;
			JSObject *attributes = JSVAL_TO_OBJECT(jsv);

			JSIdArray *arr = JS_Enumerate(cx, attributes);
			if(arr->length)
			{
				for(jsint i(0); i<arr->length; i++)
				{
					if(!JS_GetPropertyById(cx, attributes, arr->vector[i], &jsv)) return false;
					JSObject *attribute = JSVAL_TO_OBJECT(jsv);

					//им€
					if(!JS_CallFunctionName(cx, attribute, "name", 0, NULL, &jsv)) return false;
					if(!JS_ConvertArguments(cx, 1, &jsv, "s", &s)) return false;

					if(preprocessXmlValue(s, preprocessed, etetAttrName))
					{
						jsv = STRING_TO_JSVAL(JS_NewStringCopyN(cx, preprocessed.data(), preprocessed.size()));
						if(!JS_CallFunctionName(cx, attribute, "setName", 1, &jsv, &jsv)) return false;
					}

					//значение
					if(!JS_CallFunctionName(cx, attribute, "toXMLString", 0, NULL, &jsv)) return false;
					if(!JS_ConvertArguments(cx, 1, &jsv, "s", &s)) return false;

					if(preprocessXmlValue(s, preprocessed, etetAttrValue))
					{
						if(!JS_CallFunctionName(cx, attribute, "name", 0, NULL, &jsv)) return false;
						if(!JS_ConvertArguments(cx, 1, &jsv, "s", &s)) return false;
						std::string attrName = "@";
						attrName += s;

						jsv = STRING_TO_JSVAL(JS_NewStringCopyN(cx, preprocessed.data(), preprocessed.size()));
						if(!JS_SetProperty(cx, xml, attrName.data(), &jsv)) return false;
					}
				}
			}
			JS_DestroyIdArray(ecx()->_jsCx, arr);

			//дочерние
			if(!JS_CallFunctionName(cx, xml, "children", 0, NULL, &jsv)) return false;
			JSObject *children = JSVAL_TO_OBJECT(jsv);
			arr = JS_Enumerate(cx, children);
			if(arr->length)
			{
				for(jsint i(0); i<arr->length; i++)
				{
					if(!JS_GetPropertyById(cx, children, arr->vector[i], &jsv)) return false;
					JSObject *child = JSVAL_TO_OBJECT(jsv);
					if(!preprocessXml(child))
					{
						return false;
					}
				}
			}
			JS_DestroyIdArray(ecx()->_jsCx, arr);

			return true;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::preprocessXmlValue(const char *s, std::string &preprocessed, ETemplateEscaperType etet)
	{
		//регэкспом се€ть s и добавл€ть соответствующий etet префикс
		//если есть хот€бы одна замена то вернуть истину

		using namespace boost::xpressive;
		static const cregex re = 
			s1=(as_xpr(MARK_PREFIX)>>	(+_d)>>		as_xpr(MARK_SUFFIX));

		preprocessed.clear();

		const char *format;
		switch(etet)
		{
		case etetTagName:
			format = MARK_PREFIX_TAGNAME "$1";
			break;
		case etetAttrName:
			format = MARK_PREFIX_ATTRNAME "$1";
			break;
		case etetAttrValue:
			format = MARK_PREFIX_ATTRVALUE "$1";
			break;
		case etetXml:
			format = MARK_PREFIX_XML "$1";
			break;
		default:
			return false;
		}
		preprocessed = boost::xpressive::regex_replace(s, re, format);
		return preprocessed!=s;
	}


	//////////////////////////////////////////////////////////////////////////
	JSBool Template::onClassCall(uintN argc, jsval *argv, jsval *rval)//generateMarks
	{
		std::string marks;
		if(!generateMarks(argc, argv, marks))
		{
			if(!JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportError(ecx()->_jsCx, "[Template.generateMarks failed]");
			}
			return JS_FALSE;
		}
		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, marks.data(), marks.size()));
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::call_print(uintN argc, jsval *argv, jsval *rval)
	{
		if(!print(*ecx()->_out, TemplateEscaper(etetNull)))
		{
			if(!JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportError(ecx()->_jsCx, "[Template.print failed]");
			}
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::call_toString(uintN argc, jsval *argv, jsval *rval)
	{
		std::stringstream ss;
		if(!print(ss, TemplateEscaper(etetNull)))
		{
			if(!JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportError(ecx()->_jsCx, "[Template.toString failed]");
			}
			return false;
		}

		std::string str = ss.str();
		*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, str.data(), str.size()));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::xetter_target(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = JSVAL_VOID;
			return true;
		}
		if(!setTarget(*vp))
		{
			if(!JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportError(ecx()->_jsCx, "[Template.setTarget failed]");
			}
			return false;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::xetter_xml(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = JSVAL_VOID;
			return true;
		}
		jsval xml = JSVAL_VOID;
		if(JSTYPE_XML == JS_TypeOfValue(ecx()->_jsCx, *vp))
		{
			xml = *vp;
		}
		else
		{
			if(!JS_CallFunctionName(ecx()->_jsCx, JS_GetGlobalObject(ecx()->_jsCx), "XML", 1, vp, &xml))
			{
				jsval exc = JSVAL_VOID;
				if(JS_IsExceptionPending(ecx()->_jsCx))
				{
					JS_GetPendingException(ecx()->_jsCx, &exc);
					JS_ClearPendingException(ecx()->_jsCx);
				}

				char *s="";
				JS_ConvertArguments(ecx()->_jsCx, 1, &exc, "s", &s);
				JS_ReportError(ecx()->_jsCx, "[Template.xml accepting XML: [%s]]", s);
				return false;
			}
		}
		if(!setTarget(xml))
		{
			if(!JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportError(ecx()->_jsCx, "[Template.setTarget failed]");
			}
			return false;
		}
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool Template::xetter_text(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = JSVAL_VOID;
			return true;
		}
		jsval text;
		if(JSTYPE_XML == JS_TypeOfValue(ecx()->_jsCx, *vp))
		{
			if(!JS_CallFunctionName(ecx()->_jsCx, JSVAL_TO_OBJECT(*vp), "toXMLString", 0, NULL, &text)) return false;
		}
		else
		{
			text = *vp;
		}
		if(!setTarget(text))
		{
			if(!JS_IsExceptionPending(ecx()->_jsCx))
			{
				JS_ReportError(ecx()->_jsCx, "[Template.setTarget failed]");
			}
			return false;
		}
		return true;
	}

}
