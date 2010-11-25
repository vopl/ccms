

let target;
if(request.planData.forums) target = request.planData.forums[request.planData.forums.length-1];
if(!target)
{
	ui.blocks.center.push(this.render());
	ui.print();
	return;
}

if(request.params.confirm)
{
	target = orm.Forum.make(target);
	//warn('del '+target.id);
	//dumpe(target);
	target.delete();
	cache.fire('forum.forum');
	request.setStatusCode(303);
	request.pushHeader('Location', request.params.backUrl?request.params.backUrl:this.parent.path);
	return;
}

ui.blocks.center.push(this.render());
ui.print();

