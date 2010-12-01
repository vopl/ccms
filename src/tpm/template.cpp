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
		: JsObject(true, "Template")
		, _state(ets_null)
		, _topObject(JSVAL_NULL)
		, _clenchContainer(NULL)
	{
		jsRegisterMeth("print", &Template::call_print, 0);
		jsRegisterMeth("toString", &Template::call_toString, 0);

		jsRegisterMeth("compile", &Template::call_compile, 1);
		jsRegisterMeth("compileXml", &Template::call_compileXml, 1);
		jsRegisterMeth("compileText", &Template::call_compileText, 1);

		jsRegisterMeth("clone", &Template::call_clone, 0);

		jsRegisterProp("obj", &Template::xetter_obj, true);

		_state = ets_init;

		_topObject = thisJsval();

		_clenchContainer = JS_NewObject(ecx()->_jsCx, NULL, NULL, _js);
		JS_SetReservedSlot(ecx()->_jsCx, _js, 0, OBJECT_TO_JSVAL(_clenchContainer));
	}

	//////////////////////////////////////////////////////////////////////////
	Template::~Template()
	{
		BOOST_FOREACH(const TResult::value_type &s, _result.rnd)
			s.second->clearContentNoJs();

		_result.clear();
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::setTarget(jsval target)
	{
		JSContext *cx = ecx()->_jsCx;
		if(ets_init != _state)
		{
			JS_ReportError(cx, "[Template.setTarget already compiled]");
			return false;
		}

		std::string text;
		if(!targetToText(target, text))
		{
			_state = ets_init;
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


		TResult result;

		typedef std::set<TemplateSourcePtr, TemplateSource::PtrPred> TSSources;
		TSSources forDoubles;
		const char *iter = text.data();
		const char *end = text.data() + text.size();
		while(boost::xpressive::regex_search(iter, end, what, re))
		{
			const char *sbeg = iter+what.position();
			const char *send = sbeg+what.length();

			if(sbeg != iter)
			{
				TemplateSourcePtr t(new TemplateSource(this));
				t->setText(iter, sbeg-iter);

				std::pair<TSSources::iterator, bool> pb = forDoubles.insert(t);

				result.rnd.push_back(TResult::mk("", *pb.first));
			}

			size_t mark = _atost(what[5].str().data());
			assert(_result.size() > mark);
			TemplateSourcePtr t = _result.rnd[mark].second->clone();

			if(what[1].matched) t->setEtet(etetTagName);
			else if(what[2].matched) t->setEtet(etetAttrName);
			else if(what[3].matched) t->setEtet(etetAttrValue);
			else if(what[4].matched) t->setEtet(etetXml);
			else t->setEtet(etetNull);

			std::pair<TSSources::iterator, bool> pb = forDoubles.insert(t);

			assert(_result.rnd.size() > mark);
			result.rnd.push_back(TResult::mk(_result.rnd[mark].first, *pb.first));

			iter = send;
		}

		if(end != iter)
		{
			TemplateSourcePtr t(new TemplateSource(this));
			t->setText(iter, end-iter);

			std::pair<TSSources::iterator, bool> pb = forDoubles.insert(t);
			result.rnd.push_back(TResult::mk("", *pb.first));
		}

		//больше не понадобитс€
		forDoubles.clear();

		//////////////////////////////////////////////////////////////////////////
		//применить сформированные сурсы
		_result.swap(result);

		//////////////////////////////////////////////////////////////////////////
		//очистить проперт€ - данные, в них маркеры, надо теперь реальные данные
		JSIdArray *arr = JS_Enumerate(cx, thisJsobj());
		for(jsint i(0); i<arr->length; i++)
		{
			jsval jsv = JSVAL_NULL;
			if(!JS_IdToValue(cx, arr->vector[i], &jsv))return false;
			char *s = NULL;
			if(!JS_ConvertArguments(cx, 1, &jsv, "s", &s))return false;

			if(_result.assocNu.end() != _result.assocNu.find(s))
			{
				deletePermanent(s);
			}
		}
		JS_DestroyIdArray(ecx()->_jsCx, arr);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::print(JSObject *obj, std::ostream &out, const TemplateEscaper &escaper)
	{
		jsval oldTopObject = _topObject;
		_topObject = OBJECT_TO_JSVAL(obj);
		BOOST_FOREACH(const TResult::value_type &s, _result.rnd)
		{
			if(!s.second->print(obj, out, escaper))
			{
				//assert(0);
				_topObject = oldTopObject;
				return false;
			}
		}
		_topObject = oldTopObject;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	ETemplateState Template::getState()
	{
		return _state;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::targetToText(jsval target, std::string &text)
	{
		if(JSTYPE_XML == JS_TypeOfValue(ecx()->_jsCx, target))
		{
			JSClass *XMLClass = JS_GetClass(JSVAL_TO_OBJECT(target));

			//вз€ть текст
			if(!JS_CallFunctionName(ecx()->_jsCx, JSVAL_TO_OBJECT(target), "toXMLString", 0, NULL, &target))return false;
			char *data;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &target, "s", &data))
			{
				return false;
			}

			//обрамить в тэг
			std::string dataWithTag = "<stub>";
			dataWithTag += data;
			dataWithTag += "</stub>";
			target = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, dataWithTag.data(), dataWithTag.size()));

			//привести к XML
			JSObject *XML = JS_ConstructObjectWithArguments(ecx()->_jsCx, XMLClass, NULL, NULL, 1, &target);


			//препроцессить
			//подставить типизацию префиксами
			if(!preprocessXml(XML, true))
			{
				return false;
			}

			//привести в текст
			//и привести к тексту
			if(!JS_CallFunctionName(ecx()->_jsCx, XML, "toXMLString", 0, NULL, &target))return false;

			if(!JS_ConvertArguments(ecx()->_jsCx, 1, &target, "s", &data))
			{
				return false;
			}
			size_t len = strlen(data);

			//срезать тэг
			if(!strncmp("<stub>", data, 6))
			{
				target = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, data + 6, len-6-7));
			}
			else //if(!strncmp(L"<stub/>", data, 7))
			{
				target = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, "", 0));
			}

			_state = ets_xml;
		}
		else
		{
			_state = ets_text;
		}
		char *data;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &target, "s", &data))
		{
			return false;
		}
		size_t size = strlen(data);
		text.assign(data, data+size);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::preprocessXml(JSObject *xml, bool isTop)
	{
		JSContext *cx = ecx()->_jsCx;
		jsval jsv;
		char *kind = NULL;
		char *s;

		//тип узла
		//attribute, element, comment, processing-instruction, text
		if(!JS_CallFunctionName(cx, xml, "nodeKind", 0, NULL, &jsv)) return false;
		if(!JS_ConvertArguments(cx, 1, &jsv, "s", &kind)) return false;


		//////////////////////////////////////////////////////////////////////////
		if(!strcmp("text", kind))
		{
			//как текст
			std::string preprocessed;

			if(!JS_CallFunctionName(cx, xml, "toXMLString", 0, NULL, &jsv)) return false;
			if(!JS_ConvertArguments(cx, 1, &jsv, "s", &s)) return false;

			if(preprocessXmlValue(s, preprocessed, etetXml))
			{
				jsval parent;
				if(!JS_CallFunctionName(cx, xml, "parent", 0, NULL, &parent)) return false;

				jsv = STRING_TO_JSVAL(JS_NewStringCopyN(cx, preprocessed.data(), preprocessed.size()));

				jsval index;
				if(!JS_CallFunctionName(cx, xml, "childIndex", 0, NULL, &index)) return false;

				jsval argv[2] = {index, jsv};
				if(!JS_CallFunctionName(cx, JSVAL_TO_OBJECT(parent), "replace", 2, argv, &jsv)) return false;
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		if(!strcmp("element", kind))
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

// 		//////////////////////////////////////////////////////////////////////////
// 		if(!strcmp("list", kind))
// 		{
// 
// 
// 
// 			//дочерние
// 			JSIdArray *arr = JS_Enumerate(cx, xml);
// 			if(arr->length)
// 			{
// 				for(jsint i(0); i<arr->length; i++)
// 				{
// 					if(!JS_GetPropertyById(cx, xml, arr->vector[i], &jsv)) return false;
// 					JSObject *child = JSVAL_TO_OBJECT(jsv);
// 					if(!preprocessXml(child))
// 					{
// 						return false;
// 					}
// 				}
// 			}
// 			JS_DestroyIdArray(ecx()->_jsCx, arr);
// 			return true;
// 
// 
// 
// 
// 
// 
// 
// 			//дочерние
// 			if(!JS_CallFunctionName(cx, xml, "length", 0, NULL, &jsv)) return false;
// 			int32 length;
// 			if(!JS_ConvertArguments(cx, 1, &jsv, "i", &length))return false;
// 
// 			for(int32 i(0); i<length; i++)
// 			{
// 				jsval ji = INT_TO_JSVAL(i);
// 				if(!JS_CallFunctionName(cx, xml, "child", 1, &ji, &jsv)) return false;
// 				JSObject *child = JSVAL_TO_OBJECT(jsv);
// 				if(!preprocessXml(child))
// 				{
// 					return false;
// 				}
// 			}
// 
// 			return true;
// 		}
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
	bool Template::call_print(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		if(!print(obj, *ecx()->_out, TemplateEscaper(etetNull)))
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
	bool Template::call_toString(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		std::stringstream ss;
		if(!print(obj, ss, TemplateEscaper(etetNull)))
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
	bool Template::call_compile(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "[Template.compile must be called with 1 arg]");
			return false;
		}
		if(!setTarget(*argv))
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
	bool Template::call_compileXml(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "[Template.compileXml must be called with 1 arg]");
			return false;
		}
		jsval xml = JSVAL_VOID;
		if(JSTYPE_XML == JS_TypeOfValue(ecx()->_jsCx, *argv))
		{
			xml = *argv;
		}
		else
		{
			if(!JS_CallFunctionName(ecx()->_jsCx, JS_GetGlobalObject(ecx()->_jsCx), "XML", 1, argv, &xml))
			{
				jsval exc = JSVAL_VOID;
				if(JS_IsExceptionPending(ecx()->_jsCx))
				{
					JS_GetPendingException(ecx()->_jsCx, &exc);
					JS_ClearPendingException(ecx()->_jsCx);
				}

				char *s="";
				if(!JS_ConvertArguments(ecx()->_jsCx, 1, &exc, "s", &s))return false;
				JS_ReportError(ecx()->_jsCx, "[Template.compileXml accepting XML: [%s]]", s);
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
	bool Template::call_compileText(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "[Template.compileText must be called with 1 arg]");
			return false;
		}
		jsval text;
		if(JSTYPE_XML == JS_TypeOfValue(ecx()->_jsCx, *argv))
		{
			if(!JS_CallFunctionName(ecx()->_jsCx, JSVAL_TO_OBJECT(*argv), "toXMLString", 0, NULL, &text)) return false;
		}
		else
		{
			text = *argv;
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

	//////////////////////////////////////////////////////////////////////////
	bool Template::call_clone(uintN argc, jsval *argv, jsval *rval)
	{
		if(ets_xml != _state && ets_text != _state)
		{
			JS_ReportError(ecx()->_jsCx, "[Template.clone is not compiled]");
			return false;
		}

		*rval = OBJECT_TO_JSVAL(JS_NewObject(ecx()->_jsCx, NULL, thisJsobj(), NULL));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::xetter_obj(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			*vp = _topObject;
		}
		else
		{
			_topObject = *vp;
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Template::onPropAdd(JSObject *obj, jsid id, jsval *vp)
	{
		if(ets_null == _state)
		{
			return JS_TRUE;
		}

		char *idStr = NULL;
		jsval idVal;
		if(!JS_IdToValue(ecx()->_jsCx, id, &idVal)) return JS_FALSE;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &idVal, "s", &idStr)) return JS_FALSE;

		if(_result.assocNu.end() != _result.assocNu.find(idStr))
		{
			return JS_TRUE;
		}

		if(ets_init != _state)
		{
// 			JS_ReportError(ecx()->_jsCx, "[Template.onPropAdd already compiled]");
// 			return JS_FALSE;
			return JS_TRUE;
		}

		TemplateSourcePtr p(new TemplateSource(this));
		p->setProp(idVal, *vp);
		_result.rnd.push_back(TResult::mk(idStr, p));
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Template::onPropDel(JSObject *obj, jsid id, jsval *vp)
	{
		if(ets_null == _state)
		{
			return JS_TRUE;
		}

		if(ets_init != _state)
		{
			return JS_TRUE;
		}

		char *idStr = NULL;
		jsval idVal;
		if(!JS_IdToValue(ecx()->_jsCx, id, &idVal)) return JS_FALSE;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &idVal, "s", &idStr)) return JS_FALSE;

		TResult::AssocNu::iterator iter = _result.assocNu.find(idStr);
		if(_result.assocNu.end() != iter)
		{
			_result.assocNu.modify(iter, boost::lambda::_1 = TResult::mk("", iter->second));
		}
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Template::onPropSet(JSObject *obj, jsid id, jsval *vp)
	{
		if(ets_null == _state)
		{
			return JS_TRUE;
		}

		char *idStr = NULL;
		jsval idVal;
		if(!JS_IdToValue(ecx()->_jsCx, id, &idVal)) return JS_FALSE;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &idVal, "s", &idStr)) return JS_FALSE;

		TResult::AssocNu::iterator iter = _result.assocNu.find(idStr);
		if(_result.assocNu.end() != iter)
		{
			//iter->second->setJsval(*vp, ets_init != _state);
			iter->second->setProp(idVal, *vp);
		}
		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Template::onPropGet(JSObject *obj, jsid id, jsval *vp)
	{
		if(ets_null == _state)
		{
			return JS_TRUE;
		}

		JSContext *cx = ecx()->_jsCx;
		char *idStr = NULL;
		jsval idVal;
		if(!JS_IdToValue(ecx()->_jsCx, id, &idVal)) return JS_FALSE;
		if(!JS_ConvertArguments(cx, 1, &idVal, "s", &idStr)) return JS_FALSE;

		if(ets_init == _state)
		{
			TResult::AssocNu::iterator aiter = _result.assocNu.find(idStr);
			if(_result.assocNu.end() != aiter)
			{
				TResult::Rnd::iterator riter = _result.project<1>(aiter);
				size_t idx = riter - _result.rnd.begin();
				
				std::string vpStr = MARK_PREFIX;
				vpStr += _ntoa(idx);
				vpStr += MARK_SUFFIX;

				*vp = STRING_TO_JSVAL(JS_NewStringCopyN(cx, vpStr.data(), vpStr.size()));
			}
			else
			{
				JSBool b;
				if(JS_HasPropertyById(cx, thisJsobj(), id, &b) && !b)
				{
					TemplateSourcePtr p(new TemplateSource(this));
					jsval idVal;
					if(!JS_IdToValue(ecx()->_jsCx, id, &idVal)) return JS_FALSE;
					p->setProp(idVal, JSVAL_VOID);
					std::pair<TResult::Rnd::iterator,bool> pb = _result.rnd.push_back(TResult::mk(idStr, p));
					size_t idx = pb.first - _result.rnd.begin();

					std::string vpStr = MARK_PREFIX;
					vpStr += _ntoa(idx);
					vpStr += MARK_SUFFIX;

					*vp = STRING_TO_JSVAL(JS_NewStringCopyN(cx, vpStr.data(), vpStr.size()));
				}
			}
		}
		else
		{
			TResult::AssocNu::iterator aiter = _result.assocNu.find(idStr);
			if(_result.assocNu.end() != aiter)
			{
				aiter->second->getJsval(obj, *vp);
			}

		}

		return JS_TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	JSBool Template::onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval)
	{
		JSContext *cx = ecx()->_jsCx;
		if(2 == argc)
		{//named value
			char *idStr = NULL;
			if(!JS_ConvertArguments(cx, 1, &argv[0], "s", &idStr)) return JS_FALSE;

			//if(!JS_DefineProperty(cx, obj, idStr, argv[1], NULL, NULL, 0))return false;

			TemplateSourcePtr p(new TemplateSource(this));
			p->setProp(argv[0], argv[1]);
			std::pair<TResult::Rnd::iterator,bool> pb = _result.rnd.push_back(TResult::mk(idStr, p));
			size_t idx = pb.first - _result.rnd.begin();

			std::string vpStr = MARK_PREFIX;
			vpStr += _ntoa(idx);
			vpStr += MARK_SUFFIX;

			*rval = STRING_TO_JSVAL(JS_NewStringCopyN(cx, vpStr.data(), vpStr.size()));

			return JS_TRUE;
		}
		else if(1 == argc)
		{//value
			TemplateSourcePtr p(new TemplateSource(this));
			p->setJsval(argv[0], true);
			std::pair<TResult::Rnd::iterator,bool> pb = _result.rnd.push_back(TResult::mk("", p));
			size_t idx = pb.first - _result.rnd.begin();

			std::string vpStr = MARK_PREFIX;
			vpStr += _ntoa(idx);
			vpStr += MARK_SUFFIX;

			*rval = STRING_TO_JSVAL(JS_NewStringCopyN(cx, vpStr.data(), vpStr.size()));

			return JS_TRUE;
		}

		JS_ReportError(cx, "[Template.call must be called with 1 or 2 args]");
		return JS_FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::clenchJsval(TemplateSource *prov, int idx, jsval &jsv)
	{
		jsid id;
		if(!JS_ValueToId(ecx()->_jsCx, INT_TO_JSVAL((jsint)prov/2+idx), &id)) return false;
		return JS_DefinePropertyById(ecx()->_jsCx, _clenchContainer, id, jsv, NULL, NULL, 0)?true:false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool Template::unclenchJsval(TemplateSource *prov, int idx)
	{
		jsid id;
		if(!JS_ValueToId(ecx()->_jsCx, INT_TO_JSVAL((jsint)prov/2+idx), &id)) return false;
		return JS_DeletePropertyById(ecx()->_jsCx, _clenchContainer, id)?true:false;
	}

}
