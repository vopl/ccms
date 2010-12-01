let res = 
{
	childs:
	{
		elements:'./elements/conf.js',
		places:'./places/conf.js',
		skins:'./skins/conf.js',
		layouts:'./layouts/conf.js',
	},
	
	properties:
	{
		mapConfig:{value:'return {self:true}',},
		resolve:{file:'./resolve.js'},
	}

};
return res;