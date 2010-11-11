let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<form method='POST' action={this.path}>
		<input name='backUrl' type='hidden' value={t.backUrl}/>
		<input name='id' type='hidden' value={t.id}/>
		module:<input name='module' type='string'  value={t.module}/><br/>
		name:<input name='name' type='string'  value={t.name}/><br/>

		{t.operations}
		<input type='submit'/>
	</form>);
	arguments.callee.t=t;
}
t = t.clone();

t.backUrl = request.params.backUrl;

if('id' in request.params)
{
	let role = orm.Role.select('WHERE id=$1', request.params.id)[0];
	if(role)
	{
		t.id = role.id;
		t.module = role.module;
		t.name = role.name;

		let ownOperations = orm.query('SELECT ri.id, ri.module, ri.name, rr.value FROM {Operation} ri LEFT JOIN {OperationRole} rr ON (ri.id=rr.operation) WHERE rr.role=$1', role.id);
		let ownOperationsHash = {};
		ownOperations.forEach(function(v){ownOperationsHash[v.id]=v.value});
		let allOperations = orm.query('SELECT * FROM {Operation} ORDER BY id');

		t.operations = <></>;

		for each(let operation in allOperations)
		{
			t.operations += <>{operation.module+'.'+operation.name}</>;

			let select = <select name={'operation.'+operation.id}>
			</select>;

			if(operation.id in ownOperationsHash)
			{
				if(!ownOperationsHash[operation.id])
				{
					select.option += <>
						<option value='' selected="1"></option>
						<option value='allow'>allow</option>
						<option value='deny'>deny</option>
						</>;
				}
				else if('allow' == ownOperationsHash[operation.id])
				{
					select.option += <>
						<option value=''></option>
						<option value='allow' selected="1">allow</option>
						<option value='deny'>deny</option>
						</>;
				}
				else if('deny' == ownOperationsHash[operation.id])
				{
					select.option += <>
						<option value=''></option>
						<option value='allow'>allow</option>
						<option value='deny' selected="1">deny</option>
						</>;
				}
				else
				{
					select.option += <>
						<option value=''></option>
						<option value='allow'>allow</option>
						<option value='deny'>deny</option>
						<option value={ownOperationsHash[operation.id]} selected="1">{ownOperationsHash[operation.id]}</option>
						</>;
				}
			}
			else
			{
				select.option += <>
					<option value='' selected="1"></option>
					<option value='allow'>allow</option>
					<option value='deny'>deny</option>
					</>;
			}

			t.operations += select;
			t.operations += <br/>;
		}
	}
}

if(!t.operations)
{
	t.operations = <></>;

	let allOperations = orm.query('SELECT * FROM {Operation}');
	for each(let operation in allOperations)
	{
		t.operations += <>{operation.module+'.'+operation.name}</>;

		t.operations += <select name={'operation.'+operation.id}>
						<option value='' selected="1"></option>
						<option value='allow'>allow</option>
						<option value='deny'>deny</option>
						</select>;

		t.operations += <br/>;
	}
}

return t;
