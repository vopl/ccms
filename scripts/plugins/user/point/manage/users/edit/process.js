try
{
	if(request.params.id && request.params.login)
	{
		let user = orm.User.load({id:request.params.id});
		if(!user)
		{
			throw "user absent!";
		}
		
		//user.login=request.params.login;
		user.password=request.params.password;
		user.save();

		let roles = request.params.role;
		if('object' != typeof roles) roles = {};

		let ownRoles = orm.query('SELECT ur.role, r.name FROM {Role} r LEFT JOIN {UserRole} ur ON (r.id=ur.role) WHERE ur.user=$1', user.id);
		let ownRolesHash = {};
		ownRoles.forEach(function(v){ownRolesHash[v.role]=v.name});

		for(let role in roles)
		{
			if(!(role in ownRolesHash))
			{
				//addRole.push(Number(role));
				orm.exec('INSERT INTO {UserRole} ("user", "role") VALUES ($1, $2)', user.id, role);
			}
		}
		for(let role in ownRolesHash)
		{
			if(!(role in roles))
			{
				//delRole.push(Number(role));
				orm.exec('DELETE FROM {UserRole} WHERE "user"=$1 AND "role"=$2', user.id, role);

			}
		}


		if(request.params.backUrl)
		{
			request.setStatusCode(303);
			request.pushHeader('Location', request.params.backUrl?request.params.backUrl:'/');
			return;
		}
	}
}
catch(e)
{
	warn(e);
}
ui.blocks.center = this.render();
ui.print();


