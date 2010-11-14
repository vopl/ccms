
let mostForum = request.planData.forums[request.planData.forums.length-1]
let mostPost = request.planData.posts?request.planData.posts[request.planData.posts.length-1]:undefined;
if(request.params.save)
{
	let post = orm.ForumPost.make(request.params);
	post.forum_id = mostForum.id;
	if(request.planData.mode && request.planData.mode == 'add')
	{
		post.tree_pid = mostPost?mostPost.id:undefined;
	}
	else
	{
		post.id = mostPost.id;
		post.tree_pid = mostPost.tree_pid;
	}
	post.save();

	request.setStatusCode(303);
	let ppath = request.path.substr(0, request.path.lastIndexOf('/'));
	request.pushHeader('Location', request.params.backUrl?request.params.backUrl:ppath);
	return;

}
else if(request.planData.mode && request.planData.mode == 'add')
{
	ui.blocks.center.push(this.render({}));
	ui.print();
}
else
{
	ui.blocks.center.push(this.render(mostPost));
	ui.print();
}

