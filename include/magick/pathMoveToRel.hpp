#ifndef _ccms_magick_pathMoveToRel_hpp_
#define _ccms_magick_pathMoveToRel_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathMoveToRel
		: public PathBase
	{
	public:
		PathMoveToRel(const ::Magick::CoordinateList &from);
		PathMoveToRel(const PathMoveToRel &from);
		~PathMoveToRel();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathMovetoRel &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathMovetoRel> _impl;
	};
	typedef JsPtr<PathMoveToRel> PathMoveToRelPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathMoveToRelConstructor
		: public JsObject
	{
	public:
		PathMoveToRelConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
