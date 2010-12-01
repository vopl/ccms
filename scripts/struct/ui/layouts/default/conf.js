let res =
{
	super:'/ui/layouts/page',
	properties:
	{
		name:{value:'client',executable:false},
		content:{file:'content.js',executable:true},
	},
	statics:
	{
		'main.css':absFile('main.css'),
	},
};
return res;