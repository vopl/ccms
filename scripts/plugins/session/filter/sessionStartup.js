if(!router.cdStartInstance('session'))
{
	warn("failed to start persistentData instance 'session'. Already started?");
}
else
{
	var cronid = router.crontab.add(
	{
		meth:function test(cronid)
		{
			warn('sessions: ' + orm.query('SELECT COUNT(*) FROM {Session}')[0].count);
			orm.exec("DELETE FROM {Session} WHERE dtime<$1 AND remember!=TRUE OR atime<$1::timestamp-INTERVAL '6 month' AND remember=TRUE", new Date());
		},
		period:1*1000,
	});

	router.cd.global.sessionCronId = cronid;
}

