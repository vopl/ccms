#ifndef _ccms_magick_pathQuadraticCurveToAbs_hpp_
#define _ccms_magick_pathQuadraticCurveToAbs_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathQuadraticCurveToAbs
		: public PathBase
	{
	public:
		PathQuadraticCurveToAbs(const ::Magick::PathQuadraticCurvetoArgsList &from);
		PathQuadraticCurveToAbs(const PathQuadraticCurveToAbs &from);
		~PathQuadraticCurveToAbs();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathQuadraticCurvetoAbs &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathQuadraticCurvetoAbs> _impl;
	};
	typedef JsPtr<PathQuadraticCurveToAbs> PathQuadraticCurveToAbsPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathQuadraticCurveToAbsConstructor
		: public JsObject
	{
	public:
		PathQuadraticCurveToAbsConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
