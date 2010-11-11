let forum = request.planData.forums[request.planData.forums.length-1];
let forumPath = request.planData.nonPagePath?request.planData.nonPagePath:request.path;
let page = request.planData.page?request.planData.page:1;

let limit=20;
let offset = (page-1)*limit;
let topics = orm.query('SELECT * FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL ORDER BY id DESC LIMIT $2 OFFSET $3', forum.id, limit, offset);

let topicsXml = <></>;
for each(let topic in topics)
{
	let href  = forumPath+'/'+(topic.map_path?topic.map_path:topic.id);
	topicsXml += <tr><td>
		{topic.map_title}
		<a href={href}> go </a>
		<a href={href+'/edit?backUrl='+request.path}> edit </a>
		<a href={href+'/del?backUrl='+request.path}> del </a>
	</td></tr>;
}

return <>
форум {forum.map_title} <a href={request.plan[request.plan.length-1].path+'/navigate'}>navigate</a><br/>

	<table border="1">{topicsXml}</table>
	<a href={forumPath+'/addTopic?backUrl='+forumPath}>addTopic</a>
	
</>