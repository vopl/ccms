var plug=
{
	name:'session',
	
	installImpl:function()
	{
		exec("schema.js", orm, true);

		//inject points
		let sessionPre = router.createFilterInstance({
			kind:'preTransit',
			properties:
			{
				startup:{file:absFile('filter/sessionStartup.js'), executable:true},
				process:{file:absFile('filter/sessionPre.js'), executable:true},
				shutdown:{file:absFile('filter/sessionShutdown.js'), executable:true},
			}
		});
		let sessionPost = router.createFilterInstance({
			kind:'postTransit',
			properties:	
			{
				process:{file:absFile('filter/sessionPost.js'), executable:true},
			}
		});


		router.getPoint('/').setFilter('sessionPost', sessionPost);
		router.getPoint('/').setFilter('sessionPre', sessionPre);
		
		router.getPoint('/').moveFilter('sessionPre', -999999);//в начало
		router.getPoint('/').moveFilter('sessionPost', 999999);//в конец
		
		return true;
	},
	uninstallImpl:function()
	{
		router.getPoint('/').setFilter('sessionPre', null);
		router.getPoint('/').setFilter('sessionPost', null);
		
		exec("schema.js", orm, false);
		return true;
	},


	deserializeImpl:function()
	{
		exec("schema.js", orm, true);
		return true;
	},
	


	
};
plug.__proto__ = exec(this, "../basePlugin.js");
return plug;