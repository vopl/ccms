var res=
{
	properties:
	{
		title:{value:'User admin', executable:false},
		process:{file:'./process.js'},
		render:{file:'./render.js'},
	},

	childs:
	{
		users:"users/conf.js",
		roles:"roles/conf.js",
		rights:"rights/conf.js",
	}
};
return res;