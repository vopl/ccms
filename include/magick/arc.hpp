#ifndef _ccms_magick_arc_hpp_
#define _ccms_magick_arc_hpp_

#include "scripter/jsObject.hpp"
#include "magick/core.h"

namespace ccms {namespace magick
{
	//////////////////////////////////////////////////////////////////////////
	class Image;

	//////////////////////////////////////////////////////////////////////////
	class Arc
		: public JsObject
	{
	public:
		Arc(double startX, double startY, double endX, double endY, double startDegrees, double endDegrees);
		Arc(const Arc &from);
		~Arc();

		bool call_copy(uintN argc, jsval *argv, jsval *rval);

		bool xetter_startX(jsval *vp, bool isGet);
		bool xetter_startY(jsval *vp, bool isGet);

		bool xetter_endX(jsval *vp, bool isGet);
		bool xetter_endY(jsval *vp, bool isGet);

		bool xetter_startDegrees(jsval *vp, bool isGet);
		bool xetter_endDegrees(jsval *vp, bool isGet);

		const ::Magick::DrawableArc &getImpl() const;

	private:
		bool registerJs();
		boost::shared_ptr<::Magick::DrawableArc> _impl;

	};
	typedef JsPtr<Arc> ArcPtr;



	//////////////////////////////////////////////////////////////////////////
	class ArcConstructor
		: public JsObject
	{
	public:
		ArcConstructor();
		virtual JSBool onClassConstruct(JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	};

}}

#endif
