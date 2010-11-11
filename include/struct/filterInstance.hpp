#ifndef _ccms_filterInstance_hpp_
#define _ccms_filterInstance_hpp_

#include "scripter/jsObject.hpp"
#include "utils/initializable.hpp"
#include "assocSeqContainer.hpp"
#include "struct/propertyInstance.hpp"
#include "struct/withMaps.hpp"

namespace ccms
{
	class PointInstance;
	class Filter;

	enum EFilterInstanceProcessResult
	{
		efiprPartial,
		efiprComplete,
	};
	enum EFilterInstanceKind
	{
		efikUnknown,
		efikPreTransit,
		efikPreThis,
		efikPostThis,
		efikPostTransit,
	};

	//////////////////////////////////////////////////////////////////////////
	class FilterInstance;
	typedef JsPtr<FilterInstance> FilterInstancePtr;

	//////////////////////////////////////////////////////////////////////////
	class FilterInstance
		: public WithMaps<FilterInstance>
		, public Initializable
	{
	private:
		friend class Router;
		friend class Filter;
		PointInstance *_parent;

		//самого экземпляра свойства
		TMPropertyInstances		_properties;
		TSNames					_properties_hidden;

		EFilterInstanceKind _kind;
		EFilterInstanceKind getKind();

	private:
		virtual std::string propertyName4Error(PropertyInstance *prop);
		const TMPropertyInstances &getProperties();
		const TSNames &getProperties_hidden();
		void getProperties_inh(TMPropertyInstances &res);

	public:
		FilterInstance();
		virtual ~FilterInstance();

		void setParent(PointInstance *parent);

		virtual void init();
		virtual void deinit();
	};

	typedef AssocSeqContainer<std::string, FilterInstancePtr> TMFilterInstances;
}
#endif
