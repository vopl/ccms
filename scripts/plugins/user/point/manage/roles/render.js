let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<> 
	{t.list}
	<a href={this.path+'/edit?backUrl='+this.path}>add</a>
	</>);
	arguments.callee.t=t;
}
t = t.clone();
t.list = <table border="1"/>;

t.list.tr += <tr>
	<td>#</td>
	<td>category</td>
	<td>name</td>
	<td>operations</td>
	<td>operations</td>
</tr>;

let itemsOnPage = 30;
let page = request.params.page?request.params.page:1;

let limit = itemsOnPage;
let offset = limit*(page-1);

let roles = orm.Role.select('ORDER BY id LIMIT $1 OFFSET $2', limit, offset);

for each(let role in roles)
{
	let operations = orm.query('SELECT ri.module, ri.name, rr.value FROM {Operation} ri LEFT JOIN {OperationRole} rr ON (ri.id=rr.operation) WHERE rr.role=$1', role.id);
	t.list.tr += <tr>
		<td>{role.id}</td>
		<td>{role.category}</td>
		<td>{role.module+'.'+role.name}</td>
		<td>{operations.map(function(v)v.module+'.'+v.name+'='+v.value).join('; ')}</td>
		<td>
			<a href={this.path+'/edit?id='+role.id+'&backUrl='+this.path}>edit</a>
			<a href={this.path+'/del?id='+role.id+'&backUrl='+this.path}>del</a>
		</td>
	</tr>;
}

return t;
