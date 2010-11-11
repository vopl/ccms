let target = request.planData.forums[request.planData.forums.length-1];

let point = this;
let forums = orm.query("SELECT * FROM {Forum} WHERE tree_left>$1 AND tree_right <$2 AND tree_root=$3 ORDER BY tree_left", target.tree_left, target.tree_right, target.tree_root);
forums = request.planData.forums.concat(forums);

orm.Forum.linearToHierarchy(forums);

// let forumsById = {};

// for each(let forum in forums)
// {
	// forum.childs = [];
	// forumsById[forum.id] = forum;
// }

// for each(let forum in forums)
// {
	// if(forum.tree_pid)
	// {
		// forum.parent = forumsById[forum.tree_pid];
		// if(forum.parent)
		// {
			// forum.parent.childs.push(forum);
		// }
	// }
// }


return <>
	{point.properties.renderHeader(target)}
	вложенные форумы
{(function(){
	let res = <></>;
	
	for each(let forum in target.childs)
	{
		res += point.properties.renderRow(forum);
	}
	return <ul>{res}</ul>;
})()}

	{point.properties.renderFooter(target)}

<hr/>
топики разрешены: {target.allow_topic}<br/>
	{target.allow_topic?'тут статистика по топикам':''}
	

</>

