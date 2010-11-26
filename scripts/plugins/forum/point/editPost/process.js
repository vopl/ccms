
let mostForum = request.planData.forums[request.planData.forums.length-1]
let mostPost = request.planData.posts?request.planData.posts[request.planData.posts.length-1]:undefined;
if(request.params.save)
{
	let post = orm.ForumPost.make(request.params);

	if(post.id) post.load();

	if(!post.map_title) post.map_title = "никакненазваныйпосттипатемаапочемутыегоникакненазвалговори";
	if(!post.map_path) post.map_path = post.map_title+"->translit";
	if(!post.content) post.content = "блиннукакжетактемабезничеговнутрихотьбыsubjнаписалвотжелюдипошлинепишутничего";
	post.forum_id = mostForum.id;

	if(request.planData.mode && request.planData.mode == 'add')
	{
		let dbr = orm.query('SELECT MAX(page) FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL', mostForum.id);
		let mostPage = dbr[0].max || 1;
		dbr = orm.query('SELECT COUNT(*) FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL AND page=$2', mostForum.id, mostPage);
		let mostPageSize = dbr[0].count || 0;

		if(mostPageSize >= (mostForum.topics_navigate_page_size || 20))
		{
			mostPage++;
		}

		post.page = mostPage;
		post.tree_pid = mostPost?mostPost.id:undefined;
		post.ctime = new Date();
		post.user_id = user.id;
	}
	else
	{
		post.id = mostPost.id;
		post.tree_pid = mostPost.tree_pid;
	}
	post.mtime = new Date();
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

