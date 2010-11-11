#ifndef _ccms_magick_pathLineToHorizontalAbs_hpp_
#define _ccms_magick_pathLineToHorizontalAbs_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathLineToHorizontalAbs
		: public PathBase
	{
	public:
		PathLineToHorizontalAbs(double from);
		PathLineToHorizontalAbs(const PathLineToHorizontalAbs &from);
		~PathLineToHorizontalAbs();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathLinetoHorizontalAbs &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathLinetoHorizontalAbs> _impl;
	};
	typedef JsPtr<PathLineToHorizontalAbs> PathLineToHorizontalAbsPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathLineToHorizontalAbsConstructor
		: public JsObject
	{
	public:
		PathLineToHorizontalAbsConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
