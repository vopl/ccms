#include "stdafx.h"
#include "router/execContext.hpp"
#include <boost/thread/tss.hpp>

namespace ccms
{
	ExecContext g_execContext;

	ExecContext::ExecContext()
		: _router(NULL)
		, _request(NULL)
		, _scripter(NULL)
		, _jsCx(NULL)
		, _npc(NULL)
		, _cds(NULL)
		, _out(&std::cout)
		, _err(&std::cerr)
		, _currentName(NULL)
	{
	}

	ExecContext::~ExecContext()
	{
	}


	const ExecContext *ecx()
	{
		return &g_execContext;
	}

	ExecContext *ecx_rw()
	{
		return &g_execContext;
	}
}

