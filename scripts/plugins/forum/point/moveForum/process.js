let target = request.planData.forums[request.planData.forums.length-1];
if(target)
{
	target = orm.Forum.adopt(target);
	if(request.planData.mode == 'moveLeft')
	{
		let bound = orm.query('SELECT tree_left, tree_right FROM {Forum} WHERE tree_pid=$1 AND tree_left<$2 ORDER BY tree_left DESC LIMIT 1', target.tree_pid, target.tree_left);
		if(bound.length)
		{
			target.move(bound[0].tree_left);
			cache.fire('forum.forum');
		}
		request.setStatusCode(303);
		request.pushHeader('Location', request.params.backUrl?request.params.backUrl:request.plan[request.plan.length-2].path);
		return;
	}
	else if(request.planData.mode == 'moveRight')
	{
		let bound = orm.query('SELECT tree_left, tree_right FROM {Forum} WHERE tree_pid=$1 AND tree_right>$2 ORDER BY tree_right LIMIT 1', target.tree_pid, target.tree_right);
		if(bound.length)
		{
			target.move(bound[0].tree_right);
			cache.fire('forum.forum');
		}
		request.setStatusCode(303);
		request.pushHeader('Location', request.params.backUrl?request.params.backUrl:request.plan[request.plan.length-2].path);
	}
}

ui.print();
