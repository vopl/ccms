if(request.params.save)
{
	let forum = orm.Forum.make(request.params);
	forum.allow_topic = forum.allow_topic?true:false;
	forum.save();

	request.setStatusCode(303);
	request.pushHeader('Location', request.params.backUrl?request.params.backUrl:this.parent.path);
	return;

}
else
{
	if(request.planData.forums && request.planData.forums.length)
	{
		let target = request.planData.forums[request.planData.forums.length-1];
		if(('mode' in request.planData) && request.planData.mode == 'add')
		{
			ui.blocks.center.push(this.render({tree_pid:target.id}));
		}
		else
		{
			ui.blocks.center.push(this.render(target));
		}
	}
	else
	{
		ui.blocks.center.push(this.render(orm.Forum.make({})));
	}
}
ui.print();

