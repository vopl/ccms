var res=
{
	childs:
	{
		delForum:'delForum/conf.js',
		editForum:'editForum/conf.js',
		moveForum:'moveForum/conf.js',
		viewForum:'viewForum/conf.js',
		viewTopics:'viewTopics/conf.js',

		delPost:'delPost/conf.js',
		editPost:'editPost/conf.js',
		viewPost:'viewPost/conf.js',
	},

	properties:
	{
		access:{file:'./access.js'},
		process:{file:'./process.js'},
		render:{file:'./render.js'},

		ensureRootForum:{file:'./ensureRootForum.js'},
		resolveChild:{file:'./resolveChild.js'},
		map:{file:'./map.js'},
		mapConfig:{value:'return {self:true,forum:true};'},
		title:{file:'./title.js'},
		breadcrumbs:{file:'./breadcrumbs.js'},
	},

	filters:
	{

		preTransit:
		{
			kind:'preTransit',
			properties:
			{
				process:{file:'preFilter.js'},
			}
		},
/*		preThis:
		{
			kind:'preThis',
			properties:
			{
				process:{file:'preFilter.js'},
			}
		},
*/
	},



	statics:
	{
		'main.css':absFile('main.css'),
	},

};
return res;