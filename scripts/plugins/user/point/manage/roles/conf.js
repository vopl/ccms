var res=
{
	properties:
	{
		title:{value:'Roles', executable:false},
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