#ifndef _ccms_magick_pathLineToHorizontalRel_hpp_
#define _ccms_magick_pathLineToHorizontalRel_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathLineToHorizontalRel
		: public PathBase
	{
	public:
		PathLineToHorizontalRel(double from);
		PathLineToHorizontalRel(const PathLineToHorizontalRel &from);
		~PathLineToHorizontalRel();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathLinetoHorizontalRel &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathLinetoHorizontalRel> _impl;
	};
	typedef JsPtr<PathLineToHorizontalRel> PathLineToHorizontalRelPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathLineToHorizontalRelConstructor
		: public JsObject
	{
	public:
		PathLineToHorizontalRelConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
