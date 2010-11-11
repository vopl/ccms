let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<form method='POST' action={this.path}>
		<input name='backUrl' type='hidden' value={t.backUrl}/>
		<input name='id' type='hidden' value={t.id}/>
		module:<input name='module' type='string'  value={t.module}/><br/>
		name:<input name='name' type='string'  value={t.name}/><br/>
		<input type='submit'/>
	</form>);
	arguments.callee.t=t;
}
t = t.clone();

t.backUrl = request.params.backUrl;

if('id' in request.params)
{
	let right = orm.Operation.select('WHERE id=$1', request.params.id)[0];
	if(right)
	{
		t.id = right.id;
		t.module = right.module;
		t.name = right.name;
	}
}

return t;
