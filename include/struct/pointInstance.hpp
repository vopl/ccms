#ifndef _ccms_pointInstance_hpp_
#define _ccms_pointInstance_hpp_

#include "scripter/jsObject.hpp"

#include "router/contextData.hpp"
#include "router/path.hpp"

#include "struct/filterInstance.hpp"
#include "struct/propertyInstance.hpp"
#include "struct/staticInstance.hpp"
#include "struct/withMaps.hpp"

#include "assocSeqContainer.hpp"

namespace ccms
{

	class PointInstance;
	typedef JsPtr<PointInstance> PointInstancePtr;
	typedef AssocSeqContainer<std::string, PointInstancePtr> TMPointInstances;

	class Point;
	typedef JsPtr<Point> PointPtr;
	typedef AssocSeqContainer<std::string, PointPtr> TMPoints;

	//////////////////////////////////////////////////////////////////////////
	class PointInstance
		: public WithMaps<PointInstance>
		, public Initializable
	{
		friend class Router;

	private:
		//������� �����, �� ��� ����������� ���
		Path		_path;


	private:
		//////////////////////////////////////////////////////////////////////////
		//����������� ���������
		PointInstancePtr		_super;//����� �� �����������
		PointInstance			*_parent;//����� �� �����������

		//������ ���������� �������
		TMFilterInstances		_filters;
		TSNames					_filters_hidden;//���� ����� ���� ������������ �� �� ����� �� ��� ����� ���������

		//������ ���������� ��������
		TMPropertyInstances		_properties;
		TSNames					_properties_hidden;

		//������ ���������� ��������
		TMPointInstances		_childs;
		TSNames					_childs_hidden;

		TMStaticInstances		_statics;
		TSNames					_statics_hidden;

	public:
		PointInstance();
		~PointInstance();

		virtual void init();
		void initPath(const Path &path);
		void setParent(PointInstance *parent);
		virtual void deinit();

		void structChanged(const Path &where);

		PointInstancePtr point4Path(const Path &path);

		PointInstancePtr getParent();
		bool hasParent(PointInstancePtr point);

		PointInstancePtr getSuper();
		bool hasSuper(PointInstancePtr point);
		bool setSuper(PointInstancePtr super);
		const Path &getPath();

		////////////////////
		const TMFilterInstances &getFilters();
		const TSNames &getFilters_hidden();
		void getFilters_inh(TMFilterInstances &res);

		////////////////////
		const TMPropertyInstances &getProperties();
		const TSNames &getProperties_hidden();
		void getProperties_inh(TMPropertyInstances &res);

		////////////////////
		const TMPointInstances &getChilds();
		const TSNames &getChilds_hidden();
		void getChilds_inh(TMPointInstances &res);

		////////////////////
		const TMStaticInstances &getStatics();
		const TSNames &getStatics_hidden();
		void getStatics_inh(TMStaticInstances &res);

	private:
		virtual std::string propertyName4Error(PropertyInstance *prop);


	public:
		//////////////////////////////////////////////////////////////////////////
		//������ �������. ���� null ��� void �� �������

		//setSuper(path)
		bool call_setSuper(uintN argc, jsval *argv, jsval *rval);
	};
}
#endif
