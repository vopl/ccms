// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef _STDAFX_H_
#define _STDAFX_H_

//from <windows.h> macroces min and max
#define NOMINMAX

#ifdef _MSC_VER
#	include "targetver.h"
#endif

// #define BOOST_ASIO_ENABLE_BUFFER_DEBUGGING
// #define BOOST_ASIO_ENABLE_CANCELIO
// #define BOOST_ASIO_DISABLE_IOCP
#include <stdio.h>
//#include <tchar.h>



// TODO: reference additional headers your program requires here
#include <iostream>
#include <boost/smart_ptr.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/filesystem.hpp>

#include <boost/spirit/home/classic.hpp>
#include <boost/spirit/home/classic/phoenix.hpp>


#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index_container.hpp>

#include <boost/lambda/lambda.hpp>

#include <boost/foreach.hpp>



//#include <strstream>
#include <sstream>
#include <fstream>
#include <set>

#include <boost/xpressive/xpressive.hpp>

#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/local_time/tz_database.hpp>


#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>




//////////////////////////////////////////////////////////////////////////
#define USE_PROFILER 0

#ifdef _DEBUG
#	define USE_JSOBJECT_ROOTNAME 1
#else
#	define USE_JSOBJECT_ROOTNAME 0
#endif

#endif //_STDAFX_H_
