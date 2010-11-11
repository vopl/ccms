#ifndef _ccms_magick_pathMoveToAbs_hpp_
#define _ccms_magick_pathMoveToAbs_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathMoveToAbs
		: public PathBase
	{
	public:
		PathMoveToAbs(const ::Magick::CoordinateList &from);
		PathMoveToAbs(const PathMoveToAbs &from);
		~PathMoveToAbs();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathMovetoAbs &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathMovetoAbs> _impl;
	};
	typedef JsPtr<PathMoveToAbs> PathMoveToAbsPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathMoveToAbsConstructor
		: public JsObject
	{
	public:
		PathMoveToAbsConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
