#include "stdafx.h"
#include "icu/icu.hpp"
#include "icu/transliterator.hpp"

namespace ccms{ namespace icu{

	//////////////////////////////////////////////////////////////////////////
	Icu::Icu()
		: JsObject(true, "Icu")
	{
		jsval jsv;

		jsv = (new TransliteratorConstructor)->thisJsval();
		jsRegisterProp("Transliterator", jsv, true);
	}

	//////////////////////////////////////////////////////////////////////////
	Icu::~Icu()
	{

	}

}}
