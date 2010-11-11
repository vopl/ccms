#ifndef _ccms_magick_pathClosePath_hpp_
#define _ccms_magick_pathClosePath_hpp_

#include "magick/pathBase.hpp"

namespace ccms{namespace magick{

	class PathClosePath
		: public PathBase
	{
	public:
		PathClosePath();
		~PathClosePath();
		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		const ::Magick::PathClosePath &getImpl() const;
		virtual ::Magick::VPathBase &getBaseImpl(){return *_impl;}
	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::PathClosePath> _impl;
	};
	typedef JsPtr<PathClosePath> PathClosePathPtr;


	//////////////////////////////////////////////////////////////////////////
	class PathClosePathConstructor
		: public JsObject
	{
	public:
		PathClosePathConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};
}}
#endif
