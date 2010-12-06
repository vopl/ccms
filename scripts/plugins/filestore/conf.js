var plug=
{
	name:'filestore',
	container:router.createService('fileContainer'),
	
	installImpl:function()
	{
		this.container.base = router.getConfig().filestore;
		exec("schema.js", orm, true, this.container);
		this.runCron();
		return true;
	},

	uninstallImpl:function()
	{
		exec("schema.js", orm, false, this.container);
		this.stopCron();
		return true;
	},


	deserializeImpl:function()
	{
		this.container.base = router.getConfig().filestore;
		exec("schema.js", orm, true, this.container);
		this.runCron();
		return true;
	},
	


	runCron:function()
	{
		this.fileCleanupCrinId = router.crontab.add(
		{
			meth:function(cronid)
			{
				orm.FileCleanup.doCleanup();
			},
			period:1*1000,
		});
	},
	
	stopCron:function()
	{
		if(this.fileCleanupCrinId)
		{
			router.crontab.del(fileCleanupCrinId);
			delete this.fileCleanupCrinId;
		}
	}
	
};
plug.__proto__ = exec(this, "../basePlugin.js");
return plug;