let res =
{
	super:'/ui/layouts/page',
	properties:
	{
		name:{value:'client',executable:false},
		content:{file:'content.js',executable:true},
	},
};
return res;