#ifndef _ccms_magick_pathSmoothCurveToRel_hpp_
#define _ccms_magick_pathSmoothCurveToRel_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathSmoothCurveToRel
		: public PathBase
	{
	public:
		PathSmoothCurveToRel(const ::Magick::CoordinateList &from);
		PathSmoothCurveToRel(const PathSmoothCurveToRel &from);
		~PathSmoothCurveToRel();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathSmoothCurvetoRel &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathSmoothCurvetoRel> _impl;
	};
	typedef JsPtr<PathSmoothCurveToRel> PathSmoothCurveToRelPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathSmoothCurveToRelConstructor
		: public JsObject
	{
	public:
		PathSmoothCurveToRelConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
