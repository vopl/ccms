let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<> 
	<table border="1">
		<tr>
			<td>#</td>
			<td>category</td>
			<td>login</td>
			<td>roles</td>
			<td>operations</td>
		</tr>
	{t.list}
	</table>
	<a href={this.path+'/edit?backUrl='+this.path}>add</a>
	</>);
	arguments.callee.t=t;
}
t = t.clone();

let itemsOnPage = 30000;
let page = request.params.page?request.params.page:1;

let limit = itemsOnPage;
let offset = limit*(page-1);

let users = orm.User.select('ORDER BY id LIMIT $1 OFFSET $2', limit, offset);
let point = this;


let dbr = orm.query('SELECT ur.user, r.module, r.name FROM {UserRole} ur LEFT JOIN {Role} r ON (r.id=ur.role) WHERE ur.user IN(SELECT id FROM {User} ORDER BY id LIMIT $1 OFFSET $2)',limit, offset);
let userRoles={};
for each(let rec in dbr)
{
	if(!(rec.user in userRoles))
	{
		userRoles[rec.user] = [rec.module+'.'+rec.name];
	}
	else
	{
		userRoles[rec.user].push(rec.module+'.'+rec.name);
	}
}



function mkUsers()
{

	let tu = arguments.callee.t;
	if(!tu)
	{
		tu = router.createTemplate();
		tu.compile(<tr>
			<td>{tu.id}</td>
			<td>{tu.category}</td>
			<td>{tu.login}</td>
			<td>{tu.roles}</td>
			<td>
				<a href={tu.editHref}>edit</a>
				<a href={tu.delHref}>del</a>
			</td>
		</tr>
		);
		
		arguments.callee.t = tu;
	}
	
	t.list = [];
	for each(let user in users)
	{
		let tuc = tu.clone();
		tuc.id = user.id;
		tuc.category = user.category;
		tuc.login = user.login;
		if(user.id in userRoles)
			tuc.roles = userRoles[user.id].join(', ');
		// else
			// tuc.roles = '';
		tuc.editHref = point.path+'/edit?id='+user.id+'&backUrl='+point.path;
		tuc.delHref = point.path+'/del?id='+user.id+'&backUrl='+point.path;
		
		
		t.list.push(tuc);
	}
}
mkUsers();
return t;
