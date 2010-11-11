#ifndef _ccms_magick_pathSmoothQuadraticCurveToAbs_hpp_
#define _ccms_magick_pathSmoothQuadraticCurveToAbs_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathSmoothQuadraticCurveToAbs
		: public PathBase
	{
	public:
		PathSmoothQuadraticCurveToAbs(const ::Magick::CoordinateList &from);
		PathSmoothQuadraticCurveToAbs(const PathSmoothQuadraticCurveToAbs &from);
		~PathSmoothQuadraticCurveToAbs();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathSmoothQuadraticCurvetoAbs &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathSmoothQuadraticCurvetoAbs> _impl;
	};
	typedef JsPtr<PathSmoothQuadraticCurveToAbs> PathSmoothQuadraticCurveToAbsPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathSmoothQuadraticCurveToAbsConstructor
		: public JsObject
	{
	public:
		PathSmoothQuadraticCurveToAbsConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
