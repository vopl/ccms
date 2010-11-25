let target = request.planData.forums.back;

let point = this;








let forumTree = global.cache.process({
	key:'forum.tree.'+target.id,
	provider:function()
	{
		let forums = orm.query("SELECT * FROM {Forum} WHERE tree_left>$1 AND tree_right <$2 AND tree_root=$3 ORDER BY tree_left", target.tree_left, target.tree_right, target.tree_root);
		forums = request.planData.forums.concat(forums);

		orm.Forum.linearToHierarchy(forums);

		function mkPath(v)
		{
			point.properties.mkPath(v);
			for each(let f in v.childs) mkPath(f);
		}
		for each(let f in forums) mkPath(f);

		return forums;
	},
	events: ['forum.forum'],
});









return <>
	вложенные форумы
{(function(){
	let res = <></>;

	for each(let forum in target.childs)
	{
		res += point.properties.renderNavigateRow(forum);
	}
	return <ul>{res}</ul>;
})()}

	

</>

