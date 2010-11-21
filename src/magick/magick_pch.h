#ifndef _ccms_magick_pch_hpp_
#define _ccms_magick_pch_hpp_

#include "stdafx.h"

#ifdef _MSC_VER
#	define _VISUALC_
#endif


#include <Magick++/Image.h>
#include <Magick++/Color.h>
//#include <Magick++/Options.h> freebsd absent

#ifdef _MSC_VER
#	undef _VISUALC_
#	ifdef _DEBUG
#		pragma comment(lib,"CORE_DB_Magick++_.lib")
#	else
#		pragma comment(lib,"CORE_RL_Magick++_.lib")
#	endif
#endif



#endif
