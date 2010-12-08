var plug=
{
	name:'filestore',
	
	installImpl:function()
	{
		this.createFcs();
		exec("schema.js", orm, true);
		this.runCron();
		return true;
	},

	uninstallImpl:function()
	{
		exec("schema.js", orm, false);
		this.stopCron();
		delete global.fcs;
		return true;
	},


	deserializeImpl:function()
	{
		this.createFcs();
		exec("schema.js", orm, true);
		this.runCron();
		return true;
	},

	createFcs:function()
	{
		global.fcs = {};

		global.fcs.public = router.createService('fileContainer');
		global.fcs.private = router.createService('fileContainer');
		
		global.fcs.public.base = router.getConfig().filestore.public;
		global.fcs.private.base = router.getConfig().filestore.private;
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