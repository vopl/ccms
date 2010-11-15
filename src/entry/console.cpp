// ccms.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "transport/transportAsio.hpp"
#include "router/router.hpp"
#include "scripter/profiler.hpp"

#include "exceptions.hpp"
#include <signal.h>



//#include "../left/stackWalker/Stackwalker.h"


boost::function0<void> console_ctrl_function;

void onSignal(int sig)
{
//    printf("signal %d\n", sig);
	console_ctrl_function();
}



int main_(int argc, char* argv[])
{
	//setlocale(LC_ALL, "en_US.UTF-8");
	//setlocale(LC_CTYPE, "en_US.UTF-8");

	//std::cin.setf(std::ios::binary);
	//std::cout.setf(std::ios::binary);
	//std::cerr.setf(std::ios::binary);

// #ifdef WIN32
// 	SetConsoleCP(65001);
// 	SetConsoleOutputCP(65001);
// 	_setmode(_fileno(stdout), _O_BINARY);
// 	_setmode(_fileno(stderr), _O_BINARY);
// 	std::cout<<"sdfh рус\n";
// 	//system("chcp 65001");
// #endif

	try
	{
		ccms::Router *router = new ccms::Router();
		if(!router->create("../cfg/main.js"))
		{
			router->jsDestroy();
			delete router;
			//std::cout<<"unable to set cfg for router"<<std::endl;;
			return 0;
		}

		ccms::TransportAsio t(
			router->getConfigString("server.host").data(), 
			(unsigned short)router->getConfigUlong("server.port"), 
			router->getConfigUlong("server.queueSize"),
			router);

		t.set_mimeTypes(router->getConfigMapString("server.mimeTypes"));
		t.set_cronInterval(router->getConfigUlong("server.cronInterval"));

		t.set_keepaliveMax(router->getConfigUlong("server.keepaliveMax"));

		t.set_headerbufGranula(router->getConfigUlong("server.headerbufGranula"));
		t.set_headerbufLimit(router->getConfigUlong("server.headerbufLimit"));
		t.set_bodybufGranula(router->getConfigUlong("server.bodybufGranula"));
		t.set_bodybufLimit(router->getConfigUlong("server.bodybufLimit"));

		t.set_outbufGranula(router->getConfigUlong("server.outbufGranula"));

		t.set_enableLastModified(router->getConfigBool("server.enableLastModified"));
		t.set_enableETag(router->getConfigBool("server.enableETag"));


		std::string staticDiectory = router->getConfigString("server.staticDiectory");
		if(staticDiectory != "undefined") t.set_staticDirectory(staticDiectory.data());




		console_ctrl_function = boost::bind(&ccms::ITransport::stop, (ccms::ITransport *)&t);
		
		signal(SIGINT, onSignal);
		signal(SIGILL, onSignal);
		signal(SIGFPE, onSignal);
	//	signal(SIGSEGV, onSignal);
		signal(SIGTERM, onSignal);
		signal(SIGABRT, onSignal);

#ifdef WIN32
		signal(SIGBREAK, onSignal);
#else
		signal(SIGKILL, onSignal);
#endif
		
		{
			try
			{
				{
					ccms::ScripterScopedRequest ssr;
					std::string scriptName = router->getScripter().resolveLibName("startup.js");
					if(!scriptName.empty())
					{
						router->getScripter().f_execFile(scriptName);
					}
#if USE_PROFILER
					ccms::g_profiler.start();
#endif
				}
				//InitAllocCheck(ACOutput_Simple, FALSE, 0);

				//////////////////////////////////////////////////////////////////////////
				t.start();
				//////////////////////////////////////////////////////////////////////////

				{
#if USE_PROFILER
					ccms::g_profiler.stop();
#endif
					ccms::ScripterScopedRequest ssr;
					std::string scriptName = router->getScripter().resolveLibName("shutdown.js");
					if(!scriptName.empty())
					{
						router->getScripter().f_execFile(scriptName);
					}
				}
				//DeInitAllocCheck();

			}
			catch (ccms::exception &e)
			{
				std::cout<<"ccms exception: "<<e.what()<<std::endl;
			}
			catch (std::exception &e)
			{
				std::cout<<"std exception: "<<e.what()<<std::endl;
			}
			catch (...)
			{
				std::cout<<"unknown exception"<<std::endl;
			}

		}
		router->jsDestroy();
		delete router;
	}
	catch (ccms::exception &e)
	{
		std::cout<<"ccms exception: "<<e.what()<<std::endl;
	}
	catch (std::exception &e)
	{
		std::cout<<"std exception: "<<e.what()<<std::endl;
	}
	catch (...)
	{
		std::cout<<"unknown exception"<<std::endl;
	}

	return 0;
}


int main(int argc, char* argv[])
{
	return main_(argc, argv);
}
