#ifndef _ccms_magick_pathArcAbs_hpp_
#define _ccms_magick_pathArcAbs_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathArcAbs
		: public PathBase
	{
	public:
		PathArcAbs(const ::Magick::PathArcArgsList &from);
		PathArcAbs(const PathArcAbs &from);
		~PathArcAbs();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathArcAbs &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathArcAbs> _impl;
	};
	typedef JsPtr<PathArcAbs> PathArcAbsPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathArcAbsConstructor
		: public JsObject
	{
	public:
		PathArcAbsConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
