if(request.planData.forums.back.path != request.path)
{
	request.setStatusCode(303);
	request.pushHeader('Location', request.planData.forums.back.path);
	return;
}

//if(request.planData.forums.back.path != request.path)
ui.blocks.center.push(
	acl.hasRight('forum', 'view')?
		this.render():
		ui.skin.accessDenied());
ui.print();

