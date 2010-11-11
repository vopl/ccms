let res =
{
	super:'/ui/elements/element',
	properties:
	{
		name:{value:'page',executable:false},
		prolog:{file:'prolog.js',executable:true},
		epilog:{file:'epilog.js',executable:true},
	},
};
return res;