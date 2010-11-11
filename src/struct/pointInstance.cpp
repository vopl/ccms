#include "stdafx.h"
#include "struct/pointInstance.hpp"
#include "router/router.hpp"
#include "exceptions.hpp"
#include "utils/crc32.hpp"


#include "struct/withMaps.inc.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	PointInstance::PointInstance()
		: WithMaps<PointInstance>(true, "[object PointInstance]")
		, _parent(NULL)
	{
		jsRegisterMeth("setSuper", &PointInstance::call_setSuper, 1);


		jsRegisterMeth("setChild", &PointInstance::call_setMapElem<TMPointInstances, &PointInstance::_childs>, 2);
		jsRegisterMeth("hideChild", &PointInstance::call_hideMapElem<&PointInstance::_childs_hidden>, 2);
		jsRegisterMeth("renameChild", &PointInstance::call_renameMapElem<TMPointInstances, &PointInstance::_childs>, 2);
		jsRegisterMeth("moveChild", &PointInstance::call_moveMapElem<TMPointInstances, &PointInstance::_childs>, 2);




		jsRegisterMeth("setProperty", &PointInstance::call_setMapElem<TMPropertyInstances, &PointInstance::_properties>, 2);
		jsRegisterMeth("hideProperty", &PointInstance::call_hideMapElem<&PointInstance::_properties_hidden>, 2);
		jsRegisterMeth("renameProperty", &PointInstance::call_renameMapElem<TMPropertyInstances, &PointInstance::_properties>, 2);
		jsRegisterMeth("moveProperty", &PointInstance::call_moveMapElem<TMPropertyInstances, &PointInstance::_properties>, 2);



		jsRegisterMeth("setFilter", &PointInstance::call_setMapElem<TMFilterInstances, &PointInstance::_filters>, 2);
		jsRegisterMeth("hideFilter", &PointInstance::call_hideMapElem<&PointInstance::_filters_hidden>, 2);
		jsRegisterMeth("renameFilter", &PointInstance::call_renameMapElem<TMFilterInstances, &PointInstance::_filters>, 2);
		jsRegisterMeth("moveFilter", &PointInstance::call_moveMapElem<TMFilterInstances, &PointInstance::_filters>, 2);



		jsRegisterMeth("setStatic", &PointInstance::call_setMapElem<TMStaticInstances, &PointInstance::_statics>, 2);
		jsRegisterMeth("hideStatic", &PointInstance::call_hideMapElem<&PointInstance::_statics_hidden>, 2);
		jsRegisterMeth("renameStatic", &PointInstance::call_renameMapElem<TMStaticInstances, &PointInstance::_statics>, 2);
		jsRegisterMeth("moveStatic", &PointInstance::call_moveMapElem<TMStaticInstances, &PointInstance::_statics>, 2);
	}

	//////////////////////////////////////////////////////////////////////////
	PointInstance::~PointInstance()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void PointInstance::init()
	{
		if(isInited())
		{
			return;
		}

		{
			std::string path = "#pointInstance:/" + _path.string();
			unsigned crc32 = Crc32(path.c_str(), path.size());

			ContextDataPtr cd = mkp<ContextData>(new ContextData((void *)crc32), ROOTNAME);
			jsRegisterProp("cd", cd->thisJsval());
		}

		JSObject *obj_filters = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		mapInit(_filters.seq, obj_filters);
		jsRegisterProp("filters", OBJECT_TO_JSVAL(obj_filters));

		JSObject *obj_properties = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		mapInit(_properties.seq, obj_properties);
		jsRegisterProp("properties", OBJECT_TO_JSVAL(obj_properties));

		JSObject *obj_childs = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		mapInit(_childs.seq, obj_childs);
		jsRegisterProp("childs", OBJECT_TO_JSVAL(obj_childs));

		JSObject *obj_statics = JS_NewObject(ecx()->_jsCx, NULL, NULL, NULL);
		mapInit(_statics.seq, obj_statics);
		jsRegisterProp("statics", OBJECT_TO_JSVAL(obj_statics));

		if(_parent)
		{
			jsRegisterProp("parent", _parent->thisJsval());
		}

		//////////////////////////////////////////////////////////////////////////
		if(_super)
		{
			jsRegisterProp("super", _super->thisJsval());
		}
		jsRegisterProp("path", "/"+_path.string());

		Initializable::init();
	}

	//////////////////////////////////////////////////////////////////////////
	void PointInstance::initPath(const Path &path)
	{
		_path = path;
		BOOST_FOREACH(const TMPointInstances::Element &el, _childs.seq)
		{
			el.second->setParent(this);
			el.second->initPath(path/el.first);
		}

		BOOST_FOREACH(const TMStaticInstances::Element &el, _statics.seq)
		{
			el.second->setParent(this);
		}

		BOOST_FOREACH(const TMPropertyInstances::Element &el, _properties.seq)
		{
			el.second->setParent(this);
		}

		BOOST_FOREACH(const TMFilterInstances::Element &el, _filters.seq)
		{
			el.second->setParent(this);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void PointInstance::setParent(PointInstance *parent)
	{
		_parent = parent;
	}


	//////////////////////////////////////////////////////////////////////////
	void PointInstance::deinit()
	{
		if(!isInited())
		{
			return;
		}

		Initializable::deinit();


		deletePermanent("super");
		deletePermanent("path");

		deletePermanent("parent");

		deletePermanent("childs");
		mapDeinit(_childs);

		deletePermanent("statics");
		mapDeinit(_statics);

		deletePermanent("properties");
		mapDeinit(_properties);

		deletePermanent("filters");
		mapDeinit(_filters);

		getPropertyPtr<ContextData>("cd")->unregister();
		deletePermanent("cd");
	}

	//////////////////////////////////////////////////////////////////////////
	void PointInstance::structChanged(const Path &where)
	{
		//самого себя не обновлять
		if(where == getPath())
		{//собственный путь
			initPath(_path);
			reinit();
			return;
		}

		//супера
		if(_super && where == _super->getPath())
		{//супер был изменен
			initPath(_path);
			reinit();
			return;
		}

		//пробить вглубь
		TMPointInstances::Seq::iterator iter = _childs.seq.begin();
		TMPointInstances::Seq::iterator end = _childs.seq.end();
		for(; iter!=end; iter++)
		{
			iter->second->structChanged(where);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	PointInstancePtr PointInstance::point4Path(const Path &path)
	{
		if(path.empty())
		{
			return mkp(this, ROOTNAME);
		}

		TMPointInstances::iterator iter = _childs.find(path.front());
		if(_childs.end() == iter)
		{
			return PointInstancePtr();
		}

		return iter->second->point4Path(Path(path.begin()+1, path.end()));
	}

	//////////////////////////////////////////////////////////////////////////
	PointInstancePtr PointInstance::getParent()
	{
		return mkp(_parent, ROOTNAME);
	}

	//////////////////////////////////////////////////////////////////////////
	bool PointInstance::hasParent(PointInstancePtr point)
	{
		if(_parent)
		{
			if(_parent == point.get())
			{
				return true;
			}

			return _parent->hasParent(point);
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	PointInstancePtr PointInstance::getSuper()
	{
		return _super;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PointInstance::hasSuper(PointInstancePtr point)
	{
		if(_super)
		{
			if(_super == point)
			{
				return true;
			}

			return _super->hasSuper(point);
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PointInstance::setSuper(PointInstancePtr super)
	{
		if(super == _super)
		{
			return true;
		}

		if(super && !ecx()->_router->isPossibleSuperPath(getPath(), super->getPath()))
		{
			return false;
		}

		_super = super;
		//reinit();
		ecx()->_router->structChanged(_path);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	const Path &PointInstance::getPath()
	{
		return _path;
	}


	//////////////////////////////////////////////////////////////////////////
	const TMFilterInstances &PointInstance::getFilters()
	{
		return _filters;
	}

	//////////////////////////////////////////////////////////////////////////
	const TSNames &PointInstance::getFilters_hidden()
	{
		return _filters_hidden;
	}


	//////////////////////////////////////////////////////////////////////////
	void PointInstance::getFilters_inh(TMFilterInstances &res)
	{
		if(_super)
		{
			_super->getFilters_inh(res);
		}
		else
		{
			res.clear();
		}

		{
			TSNames::const_iterator iter = _filters_hidden.begin();
			TSNames::const_iterator end = _filters_hidden.end();

			for(; iter!=end; iter++)
			{
				res.erase(*iter);
			}
		}

		{
			TMFilterInstances::const_iterator iter = _filters.begin();
			TMFilterInstances::const_iterator end = _filters.end();

			for(; iter!=end; iter++)
			{
				TMFilterInstances::const_iterator riter = res.assoc.find(iter->first);
				if(res.assoc.end() != riter)
				{
					riter->second = iter->second;
				}
				else
				{
					res.seq.push_back(*iter);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	const TMPropertyInstances &PointInstance::getProperties()
	{
		return _properties;
	}

	//////////////////////////////////////////////////////////////////////////
	const TSNames &PointInstance::getProperties_hidden()
	{
		return _properties_hidden;
	}

	//////////////////////////////////////////////////////////////////////////
	void PointInstance::getProperties_inh(TMPropertyInstances &res)
	{
		if(_super)
		{
			_super->getProperties_inh(res);
		}
		else
		{
			res.clear();
		}

		{
			TSNames::const_iterator iter = _properties_hidden.begin();
			TSNames::const_iterator end = _properties_hidden.end();

			for(; iter!=end; iter++)
			{
				res.erase(*iter);
			}
		}

		{
			TMPropertyInstances::const_iterator iter = _properties.begin();
			TMPropertyInstances::const_iterator end = _properties.end();

			for(; iter!=end; iter++)
			{
				TMPropertyInstances::const_iterator riter = res.assoc.find(iter->first);
				if(res.assoc.end() != riter)
				{
					riter->second = iter->second;
				}
				else
				{
					res.seq.push_back(*iter);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	const TMPointInstances &PointInstance::getChilds()
	{
		return _childs;
	}

	//////////////////////////////////////////////////////////////////////////
	const TSNames &PointInstance::getChilds_hidden()
	{
		return _childs_hidden;
	}
	
	//////////////////////////////////////////////////////////////////////////
	void PointInstance::getChilds_inh(TMPointInstances &res)
	{
		if(_super)
		{
			_super->getChilds_inh(res);
		}
		else
		{
			res.clear();
		}

		{
			TSNames::const_iterator iter = _childs_hidden.begin();
			TSNames::const_iterator end = _childs_hidden.end();

			for(; iter!=end; iter++)
			{
				res.erase(*iter);
			}
		}

		{
			TMPointInstances::const_iterator iter = _childs.begin();
			TMPointInstances::const_iterator end = _childs.end();

			for(; iter!=end; iter++)
			{
				TMPointInstances::const_iterator riter = res.assoc.find(iter->first);
				if(res.assoc.end() != riter)
				{
					riter->second = iter->second;
				}
				else
				{
					res.seq.push_back(*iter);
				}
			}
		}
	}




	//////////////////////////////////////////////////////////////////////////
	const TMStaticInstances &PointInstance::getStatics()
	{
		return _statics;
	}

	//////////////////////////////////////////////////////////////////////////
	const TSNames &PointInstance::getStatics_hidden()
	{
		return _statics_hidden;
	}

	//////////////////////////////////////////////////////////////////////////
	void PointInstance::getStatics_inh(TMStaticInstances &res)
	{
		if(_super)
		{
			_super->getStatics_inh(res);
		}
		else
		{
			res.clear();
		}

		{
			TSNames::const_iterator iter = _statics_hidden.begin();
			TSNames::const_iterator end = _statics_hidden.end();

			for(; iter!=end; iter++)
			{
				res.erase(*iter);
			}
		}

		{
			TMStaticInstances::const_iterator iter = _statics.begin();
			TMStaticInstances::const_iterator end = _statics.end();

			for(; iter!=end; iter++)
			{
				TMStaticInstances::const_iterator riter = res.assoc.find(iter->first);
				if(res.assoc.end() != riter)
				{
					riter->second = iter->second;
				}
				else
				{
					res.seq.push_back(*iter);
				}
			}
		}
	}




















	//////////////////////////////////////////////////////////////////////////
	std::string PointInstance::propertyName4Error(PropertyInstance *prop)
	{
		std::string res = "point property at /" + _path.string();

		TMPropertyInstances::iterator iter = _properties.begin();
		TMPropertyInstances::iterator end = _properties.end();

		for(; iter!=end; iter++)
		{
			if(prop == iter->second.get())
			{
				res += ":"+iter->first;
				break;
			}
		}

		return res;
	}
























	//////////////////////////////////////////////////////////////////////////
	bool PointInstance::call_setSuper(uintN argc, jsval *argv, jsval *rval)
	{
		if(argc < 1)
		{
			JS_ReportError(ecx()->_jsCx, "[PointInstance.setSuper must be called with 1 argument]");
			return false;
		}

		if(JSVAL_IS_NULL(argv[0]) || JSVAL_IS_VOID(argv[0]))
		{
			*rval = setSuper(PointInstancePtr())?JSVAL_TRUE:JSVAL_FALSE;
			return true;
		}

		char *path;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &path))return false;

		if(!ecx()->_router->isPossibleSuperPath(getPath(), path))
		{
			JS_ReportError(ecx()->_jsCx, "[PointInstance.setSuper super path %s is not possible for %s]", path, getPath().string().c_str());
			return false;
		}

		PointInstancePtr root = ecx()->_router->getRootPointInstance();
		if(!root)
		{
			JS_ReportError(ecx()->_jsCx, "[PointInstance.setSuper router is not initialized]");
			return false;
		}

		PointInstancePtr super = root->point4Path(path);
		assert(super);

		*rval = setSuper(super)?JSVAL_TRUE:JSVAL_FALSE;
		return true;
	}
}
