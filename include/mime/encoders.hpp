#ifndef _ccms_mime_encoders_hpp_
#define _ccms_mime_encoders_hpp_

#include <js/jsapi.h>

namespace ccms{namespace mime{

	bool encode_7bit(jsval &val, const char *data=NULL, size_t len=0);
	bool encode_8bit(jsval &val, const char *data=NULL, size_t len=0);
	bool encode_qp(jsval &val, const char *data=NULL, size_t len=0);
	bool encode_base64(jsval &val, const char *data=NULL, size_t len=0);
}}

#endif
