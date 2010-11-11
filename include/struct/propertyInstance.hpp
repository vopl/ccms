#ifndef _ccms_propertyInstance_hpp_
#define _ccms_propertyInstance_hpp_

#include "scripter/jsObject.hpp"
#include "scripter/scripter.hpp"
#include "utils/initializable.hpp"
#include "assocSeqContainer.hpp"

namespace ccms
{

	class PropertyInstance;

	//////////////////////////////////////////////////////////////////////////
	class PropertyInstanceParent
	{
	public:
		virtual std::string propertyName4Error(PropertyInstance *prop)=0;
	};

	//////////////////////////////////////////////////////////////////////////
	class PropertyInstance
		: public JsObject
		, public Initializable
	{
		friend class Router;
		std::string				_kind;
		std::string				_value;
		std::string				_description;
		bool					_executable;

		boost::filesystem::path	_file;
		bool					_traceFile;

		PropertyInstanceParent	*_parent;

		time_t					_fileMTime;
		TFunctionID				_function;
	public:
		PropertyInstance();
		~PropertyInstance();

		virtual void init();
		virtual void deinit();
		void setParent(PropertyInstanceParent *parent);

		bool render(JsObject *object, uintN argc, jsval *argv, jsval *rval);

	private:
		bool call_setKind(uintN argc, jsval *argv, jsval *rval);
		bool call_setValue(uintN argc, jsval *argv, jsval *rval);
		bool call_setDescription(uintN argc, jsval *argv, jsval *rval);
		bool call_setExecutable(uintN argc, jsval *argv, jsval *rval);
		bool call_setFile(uintN argc, jsval *argv, jsval *rval);
		bool call_setTraceFile(uintN argc, jsval *argv, jsval *rval);


	private:
		void update(bool force = false);
		void dropFunction();
	};

	typedef JsPtr<PropertyInstance> PropertyInstancePtr;
	typedef AssocSeqContainer<std::string, PropertyInstancePtr> TMPropertyInstances;
}
#endif
