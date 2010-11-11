
if(request.params.name)
{
	let right = orm.Operation.make({
		id:request.params.id,
		module:request.params.module,
		name:request.params.name,
	});
	right.save();


	if(request.params.backUrl)
	{
		request.setStatusCode(303);
		request.pushHeader('Location', request.params.backUrl?request.params.backUrl:'/');
		return;
	}
}

ui.blocks.center = this.render();
ui.print();


