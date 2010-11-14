
if(request.params.name)
{
	let role = orm.Role.make({
		id:request.params.id,
		module:request.params.module,
		name:request.params.name,
	});
	role.save();



	let operations = request.params.operation;
	if('object' != typeof operations) operations = {};

	let ownOperations = orm.query('SELECT r.id, rr.value FROM {Operation} r LEFT JOIN {OperationRole} rr ON (r.id=rr.operation) WHERE rr.role=$1', role.id);
	let ownOperationsHash = {};
	ownOperations.forEach(function(v){ownOperationsHash[v.id]=v.value});

	for(let operation in operations)
	{
		if(operations[operation])
		{
			if(!(operation in ownOperationsHash))
			{
				//addRole.push(Number(role));
				orm.exec('INSERT INTO {OperationRole} ("role", "operation", "value") VALUES ($1, $2, $3)', role.id, operation, operations[operation]);
			}
			else if(ownOperationsHash[operation] != operations[operation])
			{
				orm.exec('UPDATE {OperationRole} SET "value"=$3 WHERE "role"=$1 AND "operation"=$2', role.id, operation, operations[operation]);
			}
		}
		else
		{
			orm.exec('DELETE FROM {OperationRole} WHERE "role"=$1 AND "operation"=$2', role.id, operation);
		}
		
	}
	for(let operation in ownOperationsHash)
	{
		if(!operations[operation])
		{
			//delRole.push(Number(role));
			orm.exec('DELETE FROM {OperationRole} WHERE "role"=$1 AND "operation"=$2', role.id, operation);

		}
	}


	if(request.params.backUrl)
	{
		request.setStatusCode(303);
		request.pushHeader('Location', request.params.backUrl?request.params.backUrl:'/');
		return;
	}
}

ui.blocks.center(this.render());
ui.print();


