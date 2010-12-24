let post = arguments[0];
let forum = arguments[1];

if(!post.path)
{
	post.childsById = {};
	post.childsByPath = {};
	post.title = post.map_title?post.map_title:'post #'+post.id;

	if(!post.tree_pid)
	{
		post.path = forum.path + '/' + post.map_path?post.map_path:post.id;;
	}
	else
	{
		if(!post.parent.path) this.properties.mkPath(post.parent, forum);
		post.parent.childsById[post.id] = post;
		post.pathPart = post.map_path?post.map_path:post.id;
		post.parent.childsByPath[post.pathPart] = post;
		post.path = post.parent.path + '/' + post.pathPart;
	}
	post.forum = forum;
}
