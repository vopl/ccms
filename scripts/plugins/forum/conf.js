var plug=
{
	name:'forum',
	dependences:
	{
		nsTree:0,
		user:0,
		filestore:0,
		texteditor:0,
	},
	
	operations:
	[
		{
			module:'forum',
			name:'view',
			description:'manageForums description',
			roles:['admin', 'user', 'anonymous'],
		},
		{
			module:'forum',
			name:'manageForums',
			description:'manageForums description',
			roles:['admin'],
		},
		{
			module:'forum',
			name:'managePosts',
			description:'managePosts description',
			roles:['admin'],
		},
		{
			module:'forum',
			name:'viewPost',
			description:'viewPost description',
			roles:['admin', 'user', 'anonymous'],
		},
		{
			module:'forum',
			name:'writePost',
			description:'writePost description',
			roles:['admin', 'user'],
		},
	],
	
	installImpl:function()
	{
		exec("schema.js", orm, true);

		//inject points
		let point = router.createPointInstance('point/conf.js');
		router.getPoint('/').setChild('forum', point);
		
		return true;
	},

	initializeImpl:function initializeImpl()
	{
		//внедрить операции
		let forumRoleOperations = acl.setOperations(this.operations);
		
		//разделить операции по ролям
		let roles = {};
		for(let operIdx=0; operIdx<forumRoleOperations.length; operIdx++)
		{
			let oper = this.operations[operIdx];
			let doper = forumRoleOperations[operIdx];
			for each(let rname in oper.roles)
			{
				if(!roles[rname]) roles[rname] = {drole:undefined, operations:[]};
				roles[rname].operations.push(doper);
			}
		}
		
		//создать роли
		for(let rname in roles)
		{
			roles[rname].drole = acl.setRole(
				{module:'forum', name:rname}, 
				roles[rname].operations);
		
		}
		
		//внедрить админу роль админа форума
		let admin = acl.setUser(
			{login:'admin'}, 
			[roles['admin'].drole]);

		//внедрить анониму роль анонима форума
		let anomymous = acl.setUser(
			{login:'anonymous'}, 
			[roles['anonymous'].drole]);
	},
	
	uninstallImpl:function()
	{
		exec("schema.js", orm, false);
		router.getPoint('/').setChild('user', null);
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