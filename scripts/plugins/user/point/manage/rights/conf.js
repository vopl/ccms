var res=
{
	properties:
	{
		title:{value:'Operations', executable:false},
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