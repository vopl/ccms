
namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	template <class TMap>
	void WithMaps<Target>::mapInit(TMap &m, JSObject *props4)
	{

		BOOST_FOREACH(const typename TMap::value_type &el, m)
		{
			el.second->setParent((Target *)this);
			el.second->init();

			if(props4)
			{
				(JSExceptionReporter)JS_DefineProperty(
					ecx()->_jsCx, 
					props4, 
					el.first.c_str(), 
					el.second->thisJsval(), 
					NULL, NULL, 
					JSPROP_ENUMERATE|
					JSPROP_PERMANENT|
					JSPROP_READONLY);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	template <class TMap>
	void WithMaps<Target>::mapDeinit(TMap &m)
	{
		BOOST_FOREACH(const typename TMap::Element &el, m)
		{
			el.second->deinit();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	template <class TMap>
	bool WithMaps<Target>::setMapElem(TMap &map, const char *name, const typename TMap::Value &child)
	{
		if(child)
		{
			typename TMap::Assoc::iterator iter = map.assoc.find(name);
			if(map.assoc.end() != iter)
			{
				iter->second->deinit();
				iter->second->setParent(NULL);

				child->setParent((Target *)this);
				iter->second = child;
			}
			else
			{
				child->setParent((Target *)this);
				map.seq.push_back(map.mk(name, child));
			}
		}
		else
		{
			typename TMap::Assoc::iterator iter = map.assoc.find(name);
			if(map.assoc.end() != iter)
			{
				iter->second->deinit();
				iter->second->setParent(NULL);
				map.assoc.erase(iter);
			}
		}

		ecx()->_router->structChanged(((Target *)this)->getPath());
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	bool WithMaps<Target>::hideMapElem(TSNames &names, const char *name, bool hide)
	{
		if(hide)
		{
			names.insert(name);
		}
		else
		{
			names.erase(name);
		}

		ecx()->_router->structChanged(((Target *)this)->getPath());
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	template <class TMap>
	bool WithMaps<Target>::renameMapElem(TMap &map, const char *oldName, const char *newName)
	{
		if(!Path::isValidPathPart(newName))
		{
			throw exception_badPathPart(newName);
			return false;
		}

		typename TMap::Assoc::iterator aiter = map.assoc.find(oldName);
		if(map.assoc.end() == aiter)
		{
			return false;
		}

		map.assoc.modify_key(aiter, boost::lambda::_1 = newName);

		ecx()->_router->structChanged(((Target *)this)->getPath());
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	template <class Target>
	template <class TMap>
	bool WithMaps<Target>::moveMapElem(TMap &map, const char *name, int pos)
	{
		typename TMap::Assoc::iterator aiter = map.assoc.find(name);
		if(map.assoc.end() == aiter)
		{
			return false;
		}

		typename TMap::Seq::iterator siterSrc = map.template project<1>(aiter);
		typename TMap::Seq::iterator siterDest = siterSrc;

		if(pos < 0)
		{
			while(pos<0 && siterDest!=map.seq.begin())
			{
				pos++;
				siterDest--;
			}
		}
		else
		{
			pos++;
			while(pos>0 && siterDest!=map.seq.end())
			{
				pos--;
				siterDest++;
			}
		}

		if(siterDest != siterSrc)
		{
			map.seq.relocate(siterDest, siterSrc);
		}

		ecx()->_router->structChanged(((Target *)this)->getPath());
		return true;
	}










}
