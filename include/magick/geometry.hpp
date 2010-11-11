#ifndef _ccms_magick_geometry_hpp_
#define _ccms_magick_geometry_hpp_

#include "scripter/jsObject.hpp"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class Geometry
		: public JsObject
	{

	public:
		Geometry(uint32 width_,
			uint32 height_,
			int32 xOff_ = 0,
			int32 yOff_ = 0,
			bool xNegative_ = false,
			bool yNegative_ = false );
		Geometry ( const char * geometry_ );
		Geometry ( const Geometry &geometry_ );
		Geometry ( const ::Magick::Geometry &geometry_ );
		Geometry ( );
		~Geometry ( void );


		bool xetter_width(jsval *vp, bool isGet);
		bool xetter_height(jsval *vp, bool isGet);
		bool xetter_xOff(jsval *vp, bool isGet);
		bool xetter_yOff(jsval *vp, bool isGet);
		bool xetter_xNegative(jsval *vp, bool isGet);
		bool xetter_yNegative(jsval *vp, bool isGet);
		bool xetter_percent(jsval *vp, bool isGet);
		bool xetter_aspect(jsval *vp, bool isGet);
		bool xetter_greater(jsval *vp, bool isGet);
		bool xetter_less(jsval *vp, bool isGet);
		bool xetter_isValid(jsval *vp, bool isGet);
		bool xetter_string(jsval *vp, bool isGet);


		const ::Magick::Geometry &getImpl() const;

	private:
		bool registerJs();
		boost::shared_ptr< ::Magick::Geometry> _impl;

	};
	typedef JsPtr<Geometry> GeometryPtr;



	//////////////////////////////////////////////////////////////////////////
	class GeometryConstructor
		: public JsObject
	{
	public:
		GeometryConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
		virtual JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval){return onClassConstruct(obj, argc, argv, rval);}
	};

}}

#endif
