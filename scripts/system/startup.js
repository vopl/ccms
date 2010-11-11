try
{
	global.system = exec('system.js');
	ro(global, 'system');

	system.startup();

	let systemPoint = router.createPointInstance('point/conf.js');
	let root = router.getPoint('/');
	root.setChild('system', systemPoint);
}
catch(e)
{
	rw(global, 'system');
	delete global.system;
	throw e;
}
