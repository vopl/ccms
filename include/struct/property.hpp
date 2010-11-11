#ifndef _ccms_property_hpp_
#define _ccms_property_hpp_

#include "scripter/jsObject.hpp"
#include "struct/propertyInstance.hpp"

namespace ccms
{

	class Point;
	class Property
		: public JsObject
		, public Initializable
	{
	public:
		Property(JsObject *parent, PropertyInstancePtr instance);
		~Property();

		virtual void init();
		virtual void deinit();
		const PropertyInstancePtr &getInstance();

		bool call_render(uintN argc, jsval *argv, jsval *rval);

	private:
		JSBool onClassCall(JSObject *obj, uintN argc, jsval *argv, jsval *rval);


	private:
		JsObject *_parent;
		PropertyInstancePtr _instance;
	};

	typedef JsPtr<Property> PropertyPtr;
	typedef AssocSeqContainer<std::string, PropertyPtr> TMProperties;
}
#endif
