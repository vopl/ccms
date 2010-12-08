var plug=
{
	name:'texteditor',
	dependences:
	{
		filestore:0,
		user:0,
		session:0,
	},
	
	installImpl:function()
	{
		exec("schema.js", orm, true);
		this.manager = exec('manager.js');


		//inject points
		let point = router.createPointInstance('point/conf.js');
		router.getPoint('/').setChild('texteditor', point);

		this.runCron();
		return true;
	},

	uninstallImpl:function()
	{
		this.stopCron();
		router.getPoint('/').setChild('user', null);
		exec("schema.js", orm, false);
		delete this.manager;
		return true;
	},


	deserializeImpl:function()
	{
		exec("schema.js", orm, true);
		this.manager = exec('manager.js');

		//inject points
		let point = router.createPointInstance('point/conf.js');
		router.getPoint('/').setChild('texteditor', point);

		this.runCron();
		return true;
	},


	runCron:function()
	{
		this.crinId = router.crontab.add(
		{
			meth:function(cronid)
			{
				//
			},
			period:60*60*1000,
		});
	},
	
	stopCron:function()
	{
		if(this.crinId)
		{
			router.crontab.del(crinId);
			delete this.crinId;
		}
	},
	

};
plug.__proto__ = exec(this, "../basePlugin.js");
return plug;