#ifndef _ccms_requestContext_hpp_
#define _ccms_requestContext_hpp_

#include <js/jsapi.h>
namespace ccms
{
	class Router;
	class Request;
	class Scripter;
	class NewPropsCollector;
	class ContextDataStorage;

	struct ExecContext
	{
		Router *_router;
		Request *_request;
		Scripter *_scripter;
		JSContext *_jsCx;
		NewPropsCollector *_npc;
		ContextDataStorage *_cds;

		std::ostream *_out;
		std::ostream *_err;
		const char *_currentName;

		ExecContext();
		~ExecContext();
	};

	extern const ExecContext *ecx();
}
#endif
