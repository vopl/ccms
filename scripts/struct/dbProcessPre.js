if(global.db)
{
	if('OK' != global.db.status)
	{
		let now = new Date();
		let lastReset = arguments.callee.lastReset;
		if(!lastReset || (now.getTime() - lastReset.getTime())>10*1000)
		{
			warn('database connection: ', global.db.status, ', ', now);
			try
			{
				global.db.reset();
				warn('database connection: ', global.db.status, ', ', now);
				delete arguments.callee.lastReset;
				return 'partial';
			}
			catch(e)
			{
				arguments.callee.lastReset = now;
				warn('database connection reset: ', e);
			}
		}
		request.setStatusCode(500);
		request.pushHeader('Content-type', 'text/plain');
		print('database connection lost');
		return 'complete';
	}
}

return 'partial';
