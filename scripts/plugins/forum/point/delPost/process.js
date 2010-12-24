let post = request.planData.post;
if(post)
{
	orm.ForumPost.delete(post);
	cache.fire('forum.'+request.planData.forum.id+'.post');
	if(request.planData.post == request.planData.topic)
	{
		cache.fire('forum.'+request.planData.forum.id+'.topic');
	}
	request.setStatusCode(303);
	request.pushHeader('Location', request.params.backUrl?request.params.backUrl:request.planData.topic.path);
}
ui.blocks.center.push(this.render());
ui.print();
