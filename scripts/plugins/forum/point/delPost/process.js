let topic = request.planData.posts[request.planData.posts.length-1];
if(topic)
{
	orm.ForumPost.delete(topic);
	request.setStatusCode(303);
	request.pushHeader('Location', request.params.backUrl?request.params.backUrl:request.plan[request.plan.length-2].path);
}
ui.blocks.center = this.render();
ui.print();
