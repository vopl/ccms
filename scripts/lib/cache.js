
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
	if(undefined === value)
	{
		return value;
	}
	
	router.cache.set(key, value, conf.events, conf.timeout, conf.callback);
	return value;
}

////////////////////////////////////////////////////
cache.fire = function fire(event)
{
	return router.cache.fire(event);
}

////////////////////////////////////////////////////
cache.del = function del(key)
{
	return router.cache.del(key);
}

////////////////////////////////////////////////////
cache.set = function set(key, value, events, timeout, callback)
{
	return router.cache.set(key, value, events, timeout, callback);
}

////////////////////////////////////////////////////
cache.get = function get(key)
{
	return router.cache.get(key);
}

return cache;