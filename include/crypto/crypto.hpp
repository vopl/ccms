#ifndef _ccms_crypto_crypto_hpp_
#define _ccms_crypto_crypto_hpp_

#include "scripter/jsObject.hpp"

namespace ccms {namespace crypto
{
	//////////////////////////////////////////////////////////////////////////
	class Crypto
		: public JsObject
	{
	public:
		Crypto();
		~Crypto();
	};
}}
#endif

