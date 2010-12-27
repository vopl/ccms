#ifndef _ccms_icu_transliterator_hpp_
#define _ccms_icu_transliterator_hpp_

#include "scripter/jsObject.hpp"

#include <unicode/translit.h>
namespace ccms {namespace icu
{
	//////////////////////////////////////////////////////////////////////////
	class Transliterator
		: public JsObject
	{
		::icu::Transliterator *_transliterator;

	public:
		Transliterator();
		~Transliterator();

		bool xetter_id(jsval *vp, bool isGet);

		bool call_transliterate(uintN argc, jsval *argv, jsval *rval);

	private:
	};
	typedef JsPtr<Transliterator> TransliteratorPtr;



	//////////////////////////////////////////////////////////////////////////
	class TransliteratorConstructor
		: public JsObject
	{
	public:
		TransliteratorConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
