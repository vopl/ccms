#ifndef _ccms_magick_pathLineToVerticalRel_hpp_
#define _ccms_magick_pathLineToVerticalRel_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathLineToVerticalRel
		: public PathBase
	{
	public:
		PathLineToVerticalRel(double from);
		PathLineToVerticalRel(const PathLineToVerticalRel &from);
		~PathLineToVerticalRel();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathLinetoVerticalRel &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathLinetoVerticalRel> _impl;
	};
	typedef JsPtr<PathLineToVerticalRel> PathLineToVerticalRelPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathLineToVerticalRelConstructor
		: public JsObject
	{
	public:
		PathLineToVerticalRelConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
