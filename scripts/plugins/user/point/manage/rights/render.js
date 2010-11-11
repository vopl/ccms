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
	<td>module</td>
	<td>name</td>
	<td>operations</td>
</tr>;

let itemsOnPage = 30;
let page = request.params.page?request.params.page:1;

let limit = itemsOnPage;
let offset = limit*(page-1);

let rights = orm.Operation.select('ORDER BY id LIMIT $1 OFFSET $2', limit, offset);

for each(let right in rights)
{
	t.list.tr += <tr>
		<td>{right.id}</td>
		<td>{right.category}</td>
		<td>{right.module}</td>
		<td>{right.name}</td>
		<td>
			<a href={this.path+'/edit?id='+right.id+'&backUrl='+this.path}>edit</a>
			<a href={this.path+'/del?id='+right.id+'&backUrl='+this.path}>del</a>
		</td>
	</tr>;
}

return t;
