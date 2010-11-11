#ifndef _ccms_magick_pathLineToAbs_hpp_
#define _ccms_magick_pathLineToAbs_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathLineToAbs
		: public PathBase
	{
	public:
		PathLineToAbs(const ::Magick::CoordinateList &from);
		PathLineToAbs(const PathLineToAbs &from);
		~PathLineToAbs();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathLinetoAbs &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathLinetoAbs> _impl;
	};
	typedef JsPtr<PathLineToAbs> PathLineToAbsPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathLineToAbsConstructor
		: public JsObject
	{
	public:
		PathLineToAbsConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
