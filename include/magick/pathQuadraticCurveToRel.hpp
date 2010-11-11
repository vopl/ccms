#ifndef _ccms_magick_pathQuadraticCurveToRel_hpp_
#define _ccms_magick_pathQuadraticCurveToRel_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathQuadraticCurveToRel
		: public PathBase
	{
	public:
		PathQuadraticCurveToRel(const ::Magick::PathQuadraticCurvetoArgsList &from);
		PathQuadraticCurveToRel(const PathQuadraticCurveToRel &from);
		~PathQuadraticCurveToRel();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathQuadraticCurvetoRel &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathQuadraticCurvetoRel> _impl;
	};
	typedef JsPtr<PathQuadraticCurveToRel> PathQuadraticCurveToRelPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathQuadraticCurveToRelConstructor
		: public JsObject
	{
	public:
		PathQuadraticCurveToRelConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
