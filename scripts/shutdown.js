
warn('shutdown');

if(global.db)
{
	global.db.close();
	delete global.db;
}


exec('system/shutdown.js');

