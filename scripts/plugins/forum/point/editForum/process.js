if(request.params.save)
{
	let forum;
	
	if(request.params.id)
	{
		forum = orm.Forum.load({id:request.params.id});
		forum.set(request.params);
	}
	else
	{
		forum = orm.Forum.make(request.params);
	}
	forum.topic_allow = forum.topic_allow?true:false;
	forum.adoptMapPath();


	if(request.params.image_del && forum.image_id)
	{
		orm.exec('DELETE FROM {Image} WHERE id=$1', forum.image_id);
		forum.image_id = undefined;
	}

	let image = orm.Image.load({id:forum.image_id});
	if('image' in request.params && request.params.image.nameServer)
	{
		if(!image) image = orm.Image.makeFromRequest(request.params.image, {width:undefined, height:undefined, width_thumb:100, height_thumb:100}, 'public');
		else image.setFromRequest(request.params.image, {width:undefined, height:undefined, width_thumb:100, height_thumb:100}, 'public');

		let error = image.getError();

		if(!error)
		{
			image.save();

			if(forum.image_id && forum.image_id != image.id)
			{
				orm.exec('DELETE FROM {Image} WHERE id=$1', forum.image_id);
			}
			forum.image_id = image.id;
		}
		else
		{
			warn('invalid image, notify user: '+dumps(error));
		}
	}
	forum.save();
	cache.fire('forum.forum');
	cache.fire('forum.'+forum.id);

	request.setStatusCode(303);
	request.pushHeader('Location', request.params.backUrl?request.params.backUrl:this.parent.path);
	return;

}
else
{
	if(request.planData.forum)
	{
		let target = request.planData.forum;
		if(('mode' in request.planData) && request.planData.mode == 'add')
		{
			ui.blocks.center.push(this.render(
			{
				tree_pid:target.id,
				topic_allow:true,
				topics_navigate_date:true,
				topics_navigate_page:true,
				topics_navigate_rpage:true,
				topics_navigate_page_size:20,
				topics_navigate_rpage_size:20,

			}));
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

