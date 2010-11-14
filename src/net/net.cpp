#include "stdafx.h"
#include "net/net.hpp"
#include "net/smtp.hpp"

namespace ccms{ namespace net{

	//////////////////////////////////////////////////////////////////////////
	Net::Net()
		: JsObject(true, "Net")
	{
		jsval jsv;

		jsv = (new SmtpConstructor)->thisJsval();
		jsRegisterProp("Smtp", jsv, true);
	}

	//////////////////////////////////////////////////////////////////////////
	Net::~Net()
	{

	}

}}
