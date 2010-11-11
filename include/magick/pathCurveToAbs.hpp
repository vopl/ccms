#ifndef _ccms_magick_pathCurveToAbs_hpp_
#define _ccms_magick_pathCurveToAbs_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathCurveToAbs
		: public PathBase
	{
	public:
		PathCurveToAbs(const ::Magick::PathCurveToArgsList &from);
		PathCurveToAbs(const PathCurveToAbs &from);
		~PathCurveToAbs();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathCurvetoAbs &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathCurvetoAbs> _impl;
	};
	typedef JsPtr<PathCurveToAbs> PathCurveToAbsPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathCurveToAbsConstructor
		: public JsObject
	{
	public:
		PathCurveToAbsConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
