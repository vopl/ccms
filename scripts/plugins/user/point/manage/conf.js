var res=
{
	properties:
	{
		title:{value:'User admin', executable:false},

		access:{file:'access.js'},
		process:{file:'./process.js'},
		render:{file:'./render.js'},
	},

	childs:
	{
		users:"users/conf.js",
		roles:"roles/conf.js",
	}
};
return res;