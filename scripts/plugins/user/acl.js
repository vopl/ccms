let acl = {};

////////////////////////////////////////////////////////////////////////////
acl.setOperations = function setOperations(operationsArray)
{
	let res = [];
	for each(let oper in operationsArray)
	{
		let doper = orm.Operation.select('WHERE module=$1 AND name=$2', oper.module, oper.name)[0];
		if(!doper)
		{
			warn('add operation '+oper.module+'.'+oper.name);
			doper = orm.Operation.insert(oper);
		}
		res.push(doper);
	}
	
	return res;
}

////////////////////////////////////////////////////////////////////////////
acl.setRole = function setRole(role, operationsArray)
{
	let drole = orm.Role.select('WHERE module=$1 AND name=$2', role.module, role.name)[0];
	if(!drole)
	{
		warn('add role '+role.module+'.'+role.name);
		drole = orm.Role.insert(role);
	}
	
	for each(let oper in operationsArray)
	{
		let dor = orm.OperationRole.select('WHERE "role"=$1 AND operation=$2', drole.id, oper.id)[0];
		if(!dor)
		{
			warn('assign operation '+oper.module+'.'+oper.name+' to role '+drole.module+'.'+drole.name);
			dor = orm.OperationRole.insert({role:drole.id, operation:oper.id, value:'allow'});
		}
	}
	
	return drole;
}

////////////////////////////////////////////////////////////////////////////
acl.setUser = function setUser(user, rolesArray)
{
	let duser = orm.User.select('WHERE login=$1', user.login)[0];
	if(!duser)
	{
		warn('add user '+user.login);
		duser = orm.User.insert(user);
	}
	
	for each(let role in rolesArray)
	{
		let dur = orm.UserRole.select('WHERE "user"=$1 AND "role"=$2', duser.id, role.id)[0];
		if(!dur)
		{
			warn('assign role '+role.module+'.'+role.name+' to user '+duser.login);
			dur = orm.UserRole.insert({user:duser.id, role:role.id});
		}
	}
	
	return duser;
}

acl.hasRight = function hasRight(module, operation, userIdOrNull)
{
	if(!userIdOrNull)
	{
		userIdOrNull = global.user.id;
	}
	
	let res = global.cache.process({
		key:'user.'+userIdOrNull+'.rights',
		provider:function()
		{
			let dbr = orm.query('SELECT value FROM {OperationRole} o_r \
				LEFT JOIN {Operation} o ON (o_r.operation=o.id) \
				LEFT JOIN {UserRole} u_r ON (u_r.role=o_r.role) \
				WHERE o.module=$1 AND o.name=$2 AND u_r.user=$3\
				',module, operation, userIdOrNull);
				
			let res = undefined;
			
			for each(let v in dbr)
			{
				switch(v.value)
				{
				case 'deny': return false;
				case 'allow': res=true; break;
				}
			}
			return res;
		},
		events:['user.operation', 'user.role', 'user.user'],
	});
	
	return res;
}
return acl;