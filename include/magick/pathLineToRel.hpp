#ifndef _ccms_magick_pathLineToRel_hpp_
#define _ccms_magick_pathLineToRel_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathLineToRel
		: public PathBase
	{
	public:
		PathLineToRel(const ::Magick::CoordinateList &from);
		PathLineToRel(const PathLineToRel &from);
		~PathLineToRel();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathLinetoRel &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathLinetoRel> _impl;
	};
	typedef JsPtr<PathLineToRel> PathLineToRelPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathLineToRelConstructor
		: public JsObject
	{
	public:
		PathLineToRelConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
