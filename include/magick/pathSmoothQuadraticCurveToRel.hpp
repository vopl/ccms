#ifndef _ccms_magick_pathSmoothQuadraticCurveToRel_hpp_
#define _ccms_magick_pathSmoothQuadraticCurveToRel_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathSmoothQuadraticCurveToRel
		: public PathBase
	{
	public:
		PathSmoothQuadraticCurveToRel(const ::Magick::CoordinateList &from);
		PathSmoothQuadraticCurveToRel(const PathSmoothQuadraticCurveToRel &from);
		~PathSmoothQuadraticCurveToRel();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathSmoothQuadraticCurvetoRel &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathSmoothQuadraticCurvetoRel> _impl;
	};
	typedef JsPtr<PathSmoothQuadraticCurveToRel> PathSmoothQuadraticCurveToRelPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathSmoothQuadraticCurveToRelConstructor
		: public JsObject
	{
	public:
		PathSmoothQuadraticCurveToRelConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
