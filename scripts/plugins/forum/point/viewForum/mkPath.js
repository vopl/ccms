let forum = arguments[0];
if(!forum.path)
{
	forum.childsById = {};
	forum.childsByPath = {};
	forum.title = forum.map_title?forum.map_title:'forum #' + forum.id;

	if(!forum.tree_pid)
	{
		forum.path = this.parent.path;
	}
	else
	{
		if(!forum.parent.path) this.properties.mkPath(forum.parent);
		forum.parent.childsById[forum.id] = forum;
		forum.pathPart = forum.map_path?forum.map_path:forum.id;
		forum.parent.childsByPath[forum.pathPart] = forum;
		forum.path = forum.parent.path + '/' + forum.pathPart;
	}
}
