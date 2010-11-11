if(global.system)
{
	system.shutdown();
	rw(global, 'system');
	delete global.system;
}
