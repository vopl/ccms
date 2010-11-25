let forum = arguments[0];
if(!forum.path)
{
	if(!forum.tree_pid)
	{
		forum.path = this.parent.path;
	}
	else
	{
		forum.path = forum.parent.path + '/' + (forum.map_path?forum.map_path:forum.id);
	}
	forum.title = forum.map_title?forum.map_title:'forum #' + forum.id;
}
