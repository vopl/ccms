#include "stdafx.h"
#include "mime/mime.hpp"
#include "mime/content.hpp"
#include "mime/file.hpp"
#include "mime/part.hpp"

#include "mime/headers.hpp"
#include "mime/headerAddress.hpp"
#include "mime/headerComplex.hpp"
#include "mime/headerText.hpp"


namespace ccms{ namespace mime{

	//////////////////////////////////////////////////////////////////////////
	Mime::Mime()
		: JsObject(true, "[object Mime]")
	{
		jsval jsv;

		jsv = (new ContentConstructor)->thisJsval();
		jsRegisterProp("Content", jsv, true);

		jsv = (new FileConstructor)->thisJsval();
		jsRegisterProp("File", jsv, true);

		jsv = (new PartConstructor)->thisJsval();
		jsRegisterProp("Part", jsv, true);

		jsv = (new HeadersConstructor)->thisJsval();
		jsRegisterProp("Headers", jsv, true);

		jsv = (new HeaderAddressConstructor)->thisJsval();
		jsRegisterProp("HeaderAddress", jsv, true);

		jsv = (new HeaderComplexConstructor)->thisJsval();
		jsRegisterProp("HeaderComplex", jsv, true);

		jsv = (new HeaderTextConstructor)->thisJsval();
		jsRegisterProp("HeaderText", jsv, true);
	}

	//////////////////////////////////////////////////////////////////////////
	Mime::~Mime()
	{

	}

}}
