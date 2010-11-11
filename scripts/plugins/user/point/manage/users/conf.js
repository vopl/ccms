var res=
{
	properties:
	{
		title:{value:'Users', executable:false},
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