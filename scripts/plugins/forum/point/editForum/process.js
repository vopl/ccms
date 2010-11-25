if(request.params.save)
{
	let forum = orm.Forum.make(request.params);
	forum.topic_allow = forum.topic_allow?true:false;
	forum.save();
	cache.fire('forum.forum');

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

