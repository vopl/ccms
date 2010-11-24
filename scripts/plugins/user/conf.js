var plug=
{
	name:'user',
	dependences:
	{
		session:0,
	},

	operations:
	[
		{
			module:'user',
			name:'login',
			description:'login description',
			roles:['admin', 'user', 'anonymous'],
		},
		{
			module:'user',
			name:'cabinet',
			description:'manage description',
			roles:['admin', 'user'],
		},
		{
			module:'user',
			name:'manage',
			description:'manage description',
			roles:['admin'],
		},
		{
			module:'user',
			name:'manageRoles',
			description:'manageRoles description',
			roles:['admin'],
		},
		{
			module:'user',
			name:'manageUsers',
			description:'manageUsers description',
			roles:['admin'],
		},
	],
	
	installImpl:function installImpl()
	{
		exec("schema.js", orm, true);
		
		//точки такие
		//	регистрация
		//	панель авторизации/аторизованного пользователя
		//	личный кабинет
		//	администрирование

		//inject points
		let point = router.createPointInstance('point/conf.js');

		let userSessionFilterPre = router.createFilterInstance({
			kind:'preTransit',
			properties:	{process:{file:absFile('filter/sessionProcessPre.js'), executable:true},}
		});
		let userSessionFilterPost = router.createFilterInstance({
			kind:'postTransit',
			properties:	{process:{file:absFile('filter/sessionProcessPost.js'), executable:true},}
		});


		router.getPoint('/').setChild('user', point);
		
		router.getPoint('/').setFilter('userPre', userSessionFilterPre);
		router.getPoint('/').moveFilter('userPre', 999999);//в конец
		router.getPoint('/').setFilter('userPost', userSessionFilterPost);
		router.getPoint('/').moveFilter('userPost', -999999);//в начало
		
		router.i18n.addTranslation('ru', absFile('i18n/ru/texts.po'));
		
		global.acl = exec("acl.js");
		
		return true;
	},
	
	initializeImpl:function initializeImpl()
	{
		//внедрить операции
		let operations = acl.setOperations(this.operations);
		
		//разделить операции по ролям
		let roles = {};
		for(let operIdx=0; operIdx<operations.length; operIdx++)
		{
			let oper = this.operations[operIdx];
			let doper = operations[operIdx];
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
				{module:'user', name:rname}, 
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
	
	uninstallImpl:function uninstallImpl()
	{
		router.getPoint('/').setChild('user', null);
		router.getPoint('/').setFilter('userPre', null);
		router.getPoint('/').setFilter('userPost', null);
		router.i18n.delTranslation('ru', absFile('i18n/ru/texts.po'));

		delete global.acl;
		exec("schema.js", orm, false);
		return true;
	},


	deserializeImpl:function deserializeImpl()
	{
		exec("schema.js", orm, true);
		global.acl = exec("acl.js");
		return true;
	},
	


	
};
plug.__proto__ = exec(this, "../basePlugin.js");
return plug;