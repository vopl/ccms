if(request.planData.forum.path != request.path)
{
	request.setStatusCode(303);
	request.pushHeader('Location', request.planData.forum.path);
	return;
}

//if(request.planData.forumpath != request.path)
ui.blocks.center.push(
	acl.hasRight('forum', 'view')?
		this.render():
		ui.skin.accessDenied());
ui.print();

