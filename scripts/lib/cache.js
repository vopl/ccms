
let cache = {};

////////////////////////////////////////////////////
cache.process = function process(conf)
{
	let key = conf.key;
	let value = router.cache.get(key);
	if(undefined !== value)
	{
		//warn('cache.process: '+key+': exist');
		return value;
	}
	//warn('cache.process: '+key+': not exist');
	//dumpe(conf);
	
	if(typeof conf.provider != 'function')
	{
		throw Error("cache.process: provider must be an function");
	}
	value = conf.provider();
	
	if('events' in conf)
	{
		if('timeout' in conf)
		{
			router.cache.set(key, value, conf.events, conf.timeout);
		}
		else
		{
			router.cache.set(key, value, conf.events);
		}
	}
	else
	{
		router.cache.set(key, value);
	}
	
	return value;
}

////////////////////////////////////////////////////
cache.fire = function fire(event)
{
	router.cache.fire(event);
}
return cache;