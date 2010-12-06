#include "stdafx.h"
#include "router/fileContainer.hpp"
#include <sys/stat.h>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	FileContainer::FileContainer()
		: JsObject(true, "FileContainer")
	{
		(JSExceptionReporter)jsRegisterProp("base", &FileContainer::xetter_base, false);

		(JSExceptionReporter)jsRegisterMeth("pathAbs", &FileContainer::call_pathAbs, 1);
		(JSExceptionReporter)jsRegisterMeth("pathRel", &FileContainer::call_pathRel, 1);
		(JSExceptionReporter)jsRegisterMeth("stat", &FileContainer::call_stat, 1);
		(JSExceptionReporter)jsRegisterMeth("isFile", &FileContainer::call_isFile, 1);
		(JSExceptionReporter)jsRegisterMeth("isDir", &FileContainer::call_isDir, 1);
		(JSExceptionReporter)jsRegisterMeth("mkDir", &FileContainer::call_mkDir, 1);
		(JSExceptionReporter)jsRegisterMeth("mkDir4File", &FileContainer::call_mkDir4File, 1);
		(JSExceptionReporter)jsRegisterMeth("rmDir", &FileContainer::call_rmDir, 1);
		(JSExceptionReporter)jsRegisterMeth("move", &FileContainer::call_move, 2);
		(JSExceptionReporter)jsRegisterMeth("import", &FileContainer::call_import, 2);
		(JSExceptionReporter)jsRegisterMeth("copy", &FileContainer::call_copy, 2);
		(JSExceptionReporter)jsRegisterMeth("rm", &FileContainer::call_rm, 1);

	}

	//////////////////////////////////////////////////////////////////////////
	FileContainer::~FileContainer()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::xetter_base(jsval *vp, bool isGet)
	{
		if(isGet)
		{
			std::string base = _base.string();
			JSString *str = JS_NewStringCopyN(ecx()->_jsCx, base.data(), base.size());
			if(str)
			{
				*vp = STRING_TO_JSVAL(str);
				return true;
			}

			return false;
		}
		else
		{
			char *s;
			if(!JS_ConvertArguments(ecx()->_jsCx, 1, vp, "s", &s)) return false;

			try
			{
				_base = s;
				_base = boost::filesystem::system_complete(_base).normalize();
				if(!boost::filesystem::is_directory(s))
				{
					throw 220;
				}
			}
			catch(std::exception &e)
			{
				JS_ReportError(ecx()->_jsCx, "FileContainer.base is not a valid directory (%s)", e.what());
				return false;
			}
			catch(...)
			{
				JS_ReportError(ecx()->_jsCx, "FileContainer.base is not a valid directory");
				return false;
			}
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_pathAbs(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.pathAbs must be called with 1 arg");
			return false;
		}

		boost::filesystem::path p;
		if(!mkPath(argv[0], p, true)) return false;


		std::string res = p.string();

		JSString *str = JS_NewStringCopyN(ecx()->_jsCx, res.data(), res.size());
		if(str)
		{
			*rval = STRING_TO_JSVAL(str);
			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_pathRel(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.pathRel must be called with 1 arg");
			return false;
		}

		boost::filesystem::path p;
		if(!mkPath(argv[0], p, false)) return false;

		std::string res = p.string();

		JSString *str = JS_NewStringCopyN(ecx()->_jsCx, res.data(), res.size());
		if(str)
		{
			*rval = STRING_TO_JSVAL(str);
			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_stat(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.stat must be called with 1 arg");
			return false;
		}

		boost::filesystem::path p;
		if(!mkPath(argv[0], p)) return false;

		JSObject *res = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		if(!res)
		{
			return false;
		}
		*rval = OBJECT_TO_JSVAL(res);

		struct stat st = {};
		if(::stat(p.string().c_str(), &st))
		{
			int err = errno;
			if(!JS_DefineProperty(ecx()->_jsCx, res, "error", STRING_TO_JSVAL(JS_NewStringCopyZ(ecx()->_jsCx, strerror(err))), NULL, NULL, JSPROP_ENUMERATE)) return false;
			return true;
		}

		const char *type = "unknown";
		switch(S_IFMT & st.st_mode)
		{
		case S_IFREG:
			type = "file";
			if(!JS_DefineProperty(ecx()->_jsCx, res, "size", INT_TO_JSVAL(st.st_size), NULL, NULL, JSPROP_ENUMERATE)) return false;
			break;
		case S_IFDIR:
			type = "dir";
			break;
		}
		if(!JS_DefineProperty(ecx()->_jsCx, res, "type", STRING_TO_JSVAL(JS_NewStringCopyZ(ecx()->_jsCx, type)), NULL, NULL, JSPROP_ENUMERATE)) return false;
		
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_isFile(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.isFile must be called with 1 arg");
			return false;
		}

		boost::filesystem::path p;
		if(!mkPath(argv[0], p)) return false;

		*rval = boost::filesystem::is_regular_file(p)?JSVAL_TRUE:JSVAL_FALSE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_isDir(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.isFile must be called with 1 arg");
			return false;
		}

		boost::filesystem::path p;
		if(!mkPath(argv[0], p)) return false;

		*rval = boost::filesystem::is_directory(p)?JSVAL_TRUE:JSVAL_FALSE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_mkDir(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.mkDir must be called with 1 arg");
			return false;
		}

		boost::filesystem::path p;
		if(!mkPath(argv[0], p)) return false;

		try
		{
			boost::filesystem::create_directories(p);
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.mkDir failed (%s)", e.what());
			return false;
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.mkDir failed");
			return false;
		}

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_mkDir4File(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.mkDir4File must be called with 1 arg");
			return false;
		}

		boost::filesystem::path p;
		if(!mkPath(argv[0], p)) return false;

		if(p == _base)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.mkDir4File out of container");
			return false;
		}
		p = p.parent_path();

		try
		{
			boost::filesystem::create_directories(p);
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.mkDir failed (%s)", e.what());
			return false;
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.mkDir failed");
			return false;
		}

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_rmDir(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.rmDir must be called with 1 arg");
			return false;
		}

		boost::filesystem::path p;
		if(!mkPath(argv[0], p)) return false;

		if(p == _base)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.rmDir unable to remove container directory");
			return false;
		}

		if(!boost::filesystem::is_directory(p))
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.rmDir path is not a directory");
			return false;
		}

		try
		{
			boost::filesystem::remove_all(p);
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.rmDir failed (%s)", e.what());
			return false;
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.rmDir failed");
			return false;
		}

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_move(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.move must be called with 2 args");
			return false;
		}

		boost::filesystem::path from, to;
		if(!mkPath(argv[0], from)) return false;
		if(!mkPath(argv[1], to)) return false;

		try
		{
			boost::filesystem::rename(from, to);
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.move failed (%s)", e.what());
			return false;
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.move failed");
			return false;
		}

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_import(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.import must be called with 2 args");
			return false;
		}

		boost::filesystem::path from, to;
		char *sfrom;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &sfrom)) return false;

		try
		{
			from = sfrom;
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.import failed (%s)", e.what());
			return false;
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.import failed");
			return false;
		}

		if(!mkPath(argv[1], to)) return false;

		try
		{
			boost::filesystem::rename(from, to);
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.move failed (%s)", e.what());
			return false;
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.move failed");
			return false;
		}

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_copy(uintN argc, jsval *argv, jsval *rval)
	{
		if(2 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.move must be called with 2 args");
			return false;
		}

		boost::filesystem::path from, to;
		if(!mkPath(argv[0], from)) return false;
		if(!boost::filesystem::is_regular_file(from))
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.copy first arg must be a path to an existing file");
			return false;
		}

		if(!mkPath(argv[1], to)) return false;

		try
		{
			boost::filesystem::copy_file(from, to);
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.copy failed (%s)", e.what());
			return false;
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.copy failed");
			return false;
		}

		*rval = JSVAL_TRUE;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::call_rm(uintN argc, jsval *argv, jsval *rval)
	{
		if(1 != argc)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.rm must be called with 1 arg");
			return false;
		}

		boost::filesystem::path p;
		if(!mkPath(argv[0], p)) return false;

		if(!boost::filesystem::is_regular_file(p))
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.rm path is not a file");
			return false;
		}

		try
		{
			boost::filesystem::remove(p);
		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.rm failed (%s)", e.what());
			return false;
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.rm failed");
			return false;
		}

		*rval = JSVAL_TRUE;
		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	bool FileContainer::mkPath(jsval jsv, boost::filesystem::path &res, bool abs)
	{
		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, &jsv, "s", &s)) return false;

		try
		{
			boost::filesystem::path p = s;
			p.normalize();
			if(p.has_filename() && p.filename()==".") p.remove_filename();
			if(!p.has_root_directory())
			{
				p = _base/p;
				p.normalize();
				if(p.has_filename() && p.filename()==".") p.remove_filename();
			}

			boost::filesystem::path::iterator iterP = p.begin();
			boost::filesystem::path::iterator endP = p.end();
			boost::filesystem::path::iterator iterB = _base.begin();
			boost::filesystem::path::iterator endB = _base.end();

			for(; iterP!=endP && iterB!=endB; iterP++, iterB++)
			{
				if(*iterP != *iterB)
				{
					JS_ReportError(ecx()->_jsCx, "FileContainer.pathRel path out of container (%s)", s);
					return false;
				}
			}

			if(iterB!=endB)
			{
				JS_ReportError(ecx()->_jsCx, "FileContainer.pathRel path out of container (%s)", s);
				return false;
			}

			if(!abs)
			{
				res.clear();
				for(; iterP!=endP; iterP++)
				{
					res /= *iterP;
				}
			}
			else
			{
				res = p;
			}

			return true;

		}
		catch(std::exception &e)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.pathRel failed (%s)", e.what());
			return false;
		}
		catch(...)
		{
			JS_ReportError(ecx()->_jsCx, "FileContainer.pathRel failed");
			return false;
		}

		return false;
	}

}

