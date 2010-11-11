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
		},
		{
			module:'user',
			name:'manage',
			description:'manage description',
		},
		{
			module:'user',
			name:'manageRoles',
			description:'manageRoles description',
		},
		{
			module:'user',
			name:'manageUsers',
			description:'manageUsers description',
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
		let adminRoleOperations = acl.setOperations(this.operations);

		//внедрить роль админа и дать ей операции
		let adminRole = acl.setRole(
			{module:'user', name:'admin', description:'administrator role description'}, 
			adminRoleOperations);

		//внедрить админа
		let admin = acl.setUser(
			{login:'admin', password:'admin', create_date:new Date(), status:'ok'}, 
			[adminRole]);

		//внедрить анонима
		let admin = acl.setUser(
			{login:'anonymous', password:'', create_date:new Date(), status:'internal'}, 
			[]);
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