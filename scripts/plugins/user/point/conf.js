var res=
{
	properties:
	{
		process:{file:'./process.js'},
		render:{file:'./render.js'},
	},

	childs:
	{
		authorize:'authorize/conf.js',
		cabinet:'cabinet/conf.js',
		manage:'manage/conf.js',
		register:'register/conf.js',
		restorepswd:'restorepswd/conf.js',
	},
};
return res;