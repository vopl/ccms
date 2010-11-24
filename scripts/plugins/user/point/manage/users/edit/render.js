let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<form method='POST' action={this.path}>
		<input name='backUrl' type='hidden' value={t.backUrl}/>
		<input name='id' type='hidden' value={t.id}/>
		login:<input name='login' type='string'  value={t.login}/><br/>
		password:<input name='password' type='string' value={''}/><br/>

		{t.roles}
		<input type='submit'/>
	</form>);
	arguments.callee.t=t;
}
t = t.clone();

t.backUrl = request.params.backUrl;

if('id' in request.params)
{
	let user = orm.User.select('WHERE id=$1', request.params.id)[0];
	if(user)
	{
		t.id = user.id;
		t.login = user.login;

		let ownRoles = orm.query('SELECT ur.role, r.name FROM {Role} r LEFT JOIN {UserRole} ur ON (r.id=ur.role) WHERE ur.user=$1', user.id);
		let ownRolesHash = {};
		ownRoles.forEach(function(v){ownRolesHash[v.role]=v.name});
		let allRoles = orm.query('SELECT * FROM {Role}');

		t.roles = <></>;

		for each(let role in allRoles)
		{
			t.roles += <>{role.module+'.'+role.name}</>;
			if(role.id in ownRolesHash)
			{
				t.roles += <input name={'role.'+role.id} type='checkbox' value='1' checked='true'/>;
			}
			else
			{
				t.roles += <input name={'role.'+role.id} type='checkbox' value='1'/>;
			}
		}
	}
}

if(!t.roles)
{
	t.roles = <></>;

	let allRoles = orm.query('SELECT * FROM {Role}');
	for each(let role in allRoles)
	{
		t.roles += <>{role.name}</>;
		t.roles += <input name={'role.'+role.id} type='checkbox' value='1'/>;
	}
}

return t;
