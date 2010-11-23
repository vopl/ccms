var orm = arguments[0];
var install = arguments[1];

if(!install)
{
	orm.delCategory('Operation');
	orm.delCategory('Role');
	orm.delCategory('OperationRole');

	orm.delCategory('User');
	orm.delCategory('UserRole');
	return;
}


///////////////////////////////////////////////////////////
orm.addCategory({
	name:'Operation',
	inherits:'BaseCategory',
	fields:
	{
		module:'string',
		name:'string',
		description:'string',
	},
	
	indices:
	{
		idx1:
		{
			unique:true,
			targets:['module', 'name'],
		}
	},
	
	
});


///////////////////////////////////////////////////////////
orm.addCategory({
	name:'Role',
	inherits:'BaseCategory',
	fields:
	{
		module:'string',
		name:'string',
		description:'string',
	},

	indices:
	{
		idx1:
		{
			unique:true,
			targets:['module', 'name'],
		}
	},
});

///////////////////////////////////////////////////////////
orm.addCategory({
	name:'OperationRole',
	inherits:'BaseCategory',
	fields:
	{
		value:'string',
		operation:
		{
			type:'fkey',
			target:'Operation',
		},
		role:
		{
			type:'fkey',
			target:'Role',
		},
	},

	indices:
	{
		idx1:
		{
			unique:true,
			targets:['role', 'operation'],
		}
	},
});


///////////////////////////////////////////////////////////
orm.addCategory({
	name:'User',
	inherits:'BaseCategory',
	fields:
	{
		login:'string',
		password:'string',

		email:'string',
		create_date:'date',
		login_date:'date',
		status:'string',
		/*
			email
			avatar
			createDate
			lastLoginDate
			locale (lang, timezone, dateFormat, numberFormat)
		*/
	},

	indices:
	{
		idx1:
		{
			unique:true,
			targets:['login'],
		}
	},
	
	methods:
	{
		setPassword:function(p)
		{
			let soult = hash.crc32(Date().getTime());
			this.password = soult+hash.md5(soult, p);
		},
		checkPassword:function(p)
		{
			if('string' != typeof this.password || this.password.length != 24)
			{
				return false;
			}
			
			let soult = this.password.substr(0,8);
			let hp = soult+hash.md5(soult, p);
			return this.password == hp;
		},
	},
	
});

///////////////////////////////////////////////////////////
orm.addCategory({
	name:'UserRole',
	inherits:'BaseCategory',
	fields:
	{
		user:
		{
			type:'fkey',
			target:'User',
		},
		role:
		{
			type:'fkey',
			target:'Role',
		},
	},

	indices:
	{
		idx1:
		{
			unique:true,
			targets:['user', 'role'],
		}
	},
});
