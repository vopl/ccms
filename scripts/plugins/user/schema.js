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

		data:'json',
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
		setData:function(key, value)
		{
			key = String(key).split('.');

			if(!key.length)
			{
				this.data = value;
				
			}

			if('object' != typeof(this.data) || null===this.data) this.data = {};
			let data = this.data;


			for(let i=0; i<key.length-1; i++)
			{
				let part = key[i];
				if(part in data)
				{
					if('object' != typeof(data[part]) || null===data[part]) data[part] = {};
				}
				else
				{
					data[part] = {};
				}
				data = data[part];
			}

			data[key.back] = value;

			return true;
		},

		getData:function(key)
		{
			key = String(key).split('.');
			let data = this.data;

			for each(let part in key)
			{
				if('object' == typeof(data) && null!==data && part in data)
				{
					data = data[part];
				}
				else return undefined;
			}
			return data;
		},
		setPassword:function(p)
		{
			let salt = Crypto.Rand.str_(6);
			this.password = salt+Crypto.Hash.md5_(salt, p);
		},
		checkPassword:function(p)
		{
			if('string' != typeof this.password || this.password.length != 6+22)
			{
				return this.password === p;
			}
			
			let salt = this.password.substr(0,6);
			let hp = salt+Crypto.Hash.md5_(salt, p);
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
