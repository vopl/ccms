#ifndef _ccms_magick_pathArcRel_hpp_
#define _ccms_magick_pathArcRel_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathArcRel
		: public PathBase
	{
	public:
		PathArcRel(const ::Magick::PathArcArgsList &from);
		PathArcRel(const PathArcRel &from);
		~PathArcRel();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathArcRel &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathArcRel> _impl;
	};
	typedef JsPtr<PathArcRel> PathArcRelPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathArcRelConstructor
		: public JsObject
	{
	public:
		PathArcRelConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
