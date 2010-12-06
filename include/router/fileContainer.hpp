#ifndef _ccms_fileContainer_hpp_
#define _ccms_fileContainer_hpp_

#include "scripter/jsObject.hpp"

namespace ccms
{
	class FileContainer
		: public JsObject
	{
		boost::filesystem::path _base;

	public:
		FileContainer();
		~FileContainer();

		bool xetter_base(jsval *vp, bool isGet);

		//pathAbs('a/b/../c')   /usr/ccms/filestore/a/b/c
		bool call_pathAbs(uintN argc, jsval *argv, jsval *rval);
		//pathRel('a/b/../c')   a/b/c
		bool call_pathRel(uintN argc, jsval *argv, jsval *rval);

		//stat('a/b/../c')   {type:'file', size:220}
		bool call_stat(uintN argc, jsval *argv, jsval *rval);
		bool call_isFile(uintN argc, jsval *argv, jsval *rval);
		bool call_isDir(uintN argc, jsval *argv, jsval *rval);

		bool call_mkDir(uintN argc, jsval *argv, jsval *rval);
		bool call_mkDir4File(uintN argc, jsval *argv, jsval *rval);
		bool call_rmDir(uintN argc, jsval *argv, jsval *rval);

		bool call_move(uintN argc, jsval *argv, jsval *rval);
		bool call_import(uintN argc, jsval *argv, jsval *rval);
		bool call_copy(uintN argc, jsval *argv, jsval *rval);
		bool call_rm(uintN argc, jsval *argv, jsval *rval);

	private:
		bool mkPath(jsval jsv, boost::filesystem::path &res, bool abs = true);
		bool rmEmptyDirsFor(const boost::filesystem::path &filePath, jsval flag);
	};
	typedef JsPtr<FileContainer> FileContainerPtr;

}

#endif
