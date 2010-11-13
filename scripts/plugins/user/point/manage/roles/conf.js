var res=
{
	properties:
	{
		title:{value:'Roles', executable:false},

		access:{file:'access.js'},
		process:{file:'./process.js'},
		render:{file:'./render.js'},
	},

	childs:
	{
		edit:'edit/conf.js',
		del:'del/conf.js',
	}
};
return res;