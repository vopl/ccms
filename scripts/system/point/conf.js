var res=
{
	properties:
	{
		process:{file:'./process.js'},
		render:{file:'./render.asp'},
		//map:{file:'./map.js'},
	},

	childs:
	{
		options:'options/conf.js',
		database:'database/conf.js',
	}
};
return res;