///////////////////////////////////////////////////////////
//различные утилы
exec('lib/dump.js');
exec('lib/escapers.js');
exec('lib/uniq.js');
exec('lib/i18n.js');
exec('lib/stdext.js');


// база данных
global.db = router.createService('pg');
db.open(router.getConfig().dbConnectionString);
//Valid values are DEBUG5, DEBUG4, DEBUG3, DEBUG2, DEBUG1, LOG, NOTICE, WARNING, ERROR, FATAL, and PANIC
db.exec("SET client_min_messages='WARNING'");
//db.verbose=true;

// orm
global.orm = exec('lib/orm/orm.js');
orm.setDb(db);
orm.setSchema('orm');

//
exec('system/startup.js');




if(!router.plugins.session.isInstalled)
{
	router.plugins.session.install();
}
if(!router.plugins.user.isInstalled)
{
	router.plugins.user.install();
}
if(!router.plugins.nsTree.isInstalled)
{
	router.plugins.nsTree.install();
}
if(!router.plugins.forum.isInstalled)
{
	router.plugins.forum.install();
}



let options = 
{
	createSchema:'allow',
	createSequence:'allow',
	dropTable:'allow',
	dropInheritance:'allow',
	dropField:'allow',
	dropIndex:'allow',
	dropConstraint:'allow',
	
	createTable:'allow',
	createInheritance:'allow',
	createField:'allow',
	alterField:'allow',
	createIndex:'allow',
	createConstraint:'allow',
	
	fillDefault:'allow',
};

orm.sync2db(options);

for each(let plug in router.plugins)
{
	if(plug.isInstalled && plug.initialize)
	{
		plug.initialize();
	}
}

warn('startup');


// let user = orm.User.select('WHERE id=$1', 1)[0];

// let res = user.hasRight('myModule', 'myName');
// let beg = new Date();
// let cnt = 10000;
// for(let i=0; i<cnt; i++)
// {
	// res = user.hasRight('myModule', 'myName');
// }
// let end = new Date();
// dumpe(res);
// warn(cnt / (end.getTime() - beg.getTime())*1000);
