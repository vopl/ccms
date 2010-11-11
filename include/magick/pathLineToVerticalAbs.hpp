#ifndef _ccms_magick_pathLineToVerticalAbs_hpp_
#define _ccms_magick_pathLineToVerticalAbs_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathLineToVerticalAbs
		: public PathBase
	{
	public:
		PathLineToVerticalAbs(double from);
		PathLineToVerticalAbs(const PathLineToVerticalAbs &from);
		~PathLineToVerticalAbs();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathLinetoVerticalAbs &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathLinetoVerticalAbs> _impl;
	};
	typedef JsPtr<PathLineToVerticalAbs> PathLineToVerticalAbsPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathLineToVerticalAbsConstructor
		: public JsObject
	{
	public:
		PathLineToVerticalAbsConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
