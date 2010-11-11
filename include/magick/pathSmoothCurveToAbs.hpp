#ifndef _ccms_magick_pathSmoothCurveToAbs_hpp_
#define _ccms_magick_pathSmoothCurveToAbs_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathSmoothCurveToAbs
		: public PathBase
	{
	public:
		PathSmoothCurveToAbs(const ::Magick::CoordinateList &from);
		PathSmoothCurveToAbs(const PathSmoothCurveToAbs &from);
		~PathSmoothCurveToAbs();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathSmoothCurvetoAbs &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathSmoothCurvetoAbs> _impl;
	};
	typedef JsPtr<PathSmoothCurveToAbs> PathSmoothCurveToAbsPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathSmoothCurveToAbsConstructor
		: public JsObject
	{
	public:
		PathSmoothCurveToAbsConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
