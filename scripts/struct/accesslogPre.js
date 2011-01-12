let logpath = router.getConfig().accesslog;
if(logpath)
{
	if(!global.access)
	{
		global.access = router.createService('log');
		global.access.open(logpath);
	}

	global.access.print('{{{ begin ', (new Date()).ts, "\r\n");

	let logRec = {};
	logRec.path = request.path;
	logRec.env = request.env;
	logRec.cookies = request.cookies;
	logRec.paramsGet = request.paramsGet;
	logRec.paramsPost = request.paramsPost;
	logRec.files = request.files;
	global.access.print(dumps(logRec), "\r\n");
}
