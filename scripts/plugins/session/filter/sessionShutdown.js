if(!router.cdStopInstance('session'))
{
	warn("failed to stop persistentData instance 'session'. Already stopped?");
}
else
{
	//if(router.cd.global)
	{
		//if(router.cd.global.sessionCronId)
		{
			router.crontab.del(router.cd.global.sessionCronId);
			delete router.cd.global.sessionCronId;
		}
	}
}
