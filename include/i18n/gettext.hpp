#ifndef _ccms_gettext_h_
#define _ccms_gettext_h_
#include <locale.h>
#include "../../left/gettext-0.14/include/libintl.h"

namespace ccms
{
	class Gettext
	{
	public:
		Gettext()
		{
			setlocale (LC_ALL, "");
			_putenv_s("LANGUAGE", "nl_NL");

			char str[1024];
			//libintl_set_relocation_prefix("", "");

			
			char core[] = "core";
			char struc[] = "struct";
			char *csz;
			csz = getenv("LANGUAGE");

			csz = bindtextdomain (core, "../../locale/");
			bind_textdomain_codeset(core, "UTF-8");

			csz = textdomain(NULL);
			csz = textdomain(core);
			csz = gettext("AtomPub services are disabled on this blog.  An admin user can enable them at %s");
			csz = bindtextdomain (core, "");
			csz = gettext("AtomPub services are disabled on this blog.  An admin user can enable them at %s");

			int k=220;



		}
		~Gettext()
		{

		}
	};
}

static ccms::Gettext t;
#endif
