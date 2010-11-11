#ifndef _ccms_magick_pathCurveToRel_hpp_
#define _ccms_magick_pathCurveToRel_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathCurveToRel
		: public PathBase
	{
	public:
		PathCurveToRel(const ::Magick::PathCurveToArgsList &from);
		PathCurveToRel(const PathCurveToRel &from);
		~PathCurveToRel();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathCurvetoRel &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathCurvetoRel> _impl;
	};
	typedef JsPtr<PathCurveToRel> PathCurveToRelPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathCurveToRelConstructor
		: public JsObject
	{
	public:
		PathCurveToRelConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
