﻿var res=
{
	filters:
	{
		freePre:
		{
			kind:'preTransit',
			properties:
			{
				process:{file:'freePre.js', executeble:true},
			}
		},
		accesslogPre:
		{
			kind:'preTransit',
			properties:
			{
				process:{file:'accesslogPre.js', executeble:true},
			}
		},

		dbPre:
		{
			kind:'preTransit',
			properties:
			{
				process:{file:'dbProcessPre.js', executeble:true},
			}
		},


		uiPre:
		{
			kind:'preTransit',
			properties:
			{
				process:{file:'uiPre.js', executeble:true},
			}
		},
/*
		sessionPre:
		{
			kind:'preTransit',
			properties:
			{
				startup:{file:'sessionStartup.js', executeble:true},
				process:{file:'sessionProcessPre.js', executeble:true},
			}
		},
		sessionPost:
		{
			kind:'postTransit',
			properties:
			{
				process:{file:'sessionProcessPost.js', executeble:true},
				shutdown:{file:'sessionShutdown.js', executeble:true},
			}
		},
*/
		uiPost:
		{
			kind:'postTransit',
			properties:
			{
				process:{file:'uiPost.js', executeble:true},
			}
		},
		dbPost:
		{
			kind:'postTransit',
			properties:
			{
				process:{file:'dbProcessPost.js', executeble:true},
			}
		},
		accesslogPost:
		{
			kind:'postTransit',
			properties:
			{
				process:{file:'accesslogPost.js', executeble:true},
			}
		},


	},
	properties:
	{
		render://значение объявляется так же как для проперти
		{
			file:'render.js',
			executable:true,
		},

		process:{file:'process.js',executable:true},
		title:{value:"main page", executable:false},
	},
	childs:
	{
		ui:'ui/conf.js',
		dumper:'dumper/conf.js',
		test:'test/conf.js',
	},
};
return res;