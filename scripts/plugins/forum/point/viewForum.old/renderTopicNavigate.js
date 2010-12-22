let target = request.planData.forum;

if(!target.topic_allow)
{
	return <></>;
}
let forumPath = request.planData.nonPagePath?request.planData.nonPagePath:request.path;
let page = request.planData.page?request.planData.page:1;



let topicsXml = <></>;
{
	let topicsInf = orm.query('SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1', target.id)[0].count;
	let topics24 = orm.query("SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 AND ctime>(now()-interval '24 hours')", target.id)[0].count;
	let topics168 = orm.query("SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 AND ctime>(now()-interval '168 hours')", target.id)[0].count;

	let postsInf = orm.query('SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NOT NULL AND forum_id=$1', target.id)[0].count;
	let posts24 = orm.query("SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NOT NULL AND forum_id=$1 AND ctime>(now()-interval '24 hours')", target.id)[0].count;
	let posts168 = orm.query("SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NOT NULL AND forum_id=$1 AND ctime>(now()-interval '168 hours')", target.id)[0].count;

	let topicLast = orm.query("SELECT * FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 ORDER BY ctime DESC LIMIT 1", target.id)[0];
	let topicLastXml;
	if(topicLast)
	{
		let topicUserLast = orm.query("SELECT * FROM {User} WHERE id=$1", topicLast.user_id)[0];

		topicLastXml = <>
			{topicLast.ctime.ts}, {topicUserLast.login}, <a href={target.path+'/'+topicLast.map_path}>{topicLast.map_title}</a>
		</>
	}
	let postLast = orm.query("SELECT * FROM {ForumPost} WHERE tree_pid IS NOT NULL AND forum_id=$1 ORDER BY ctime DESC LIMIT 1", target.id)[0];
	let postLastXml;
	if(postLast)
	{
		let postUserLast = orm.query("SELECT * FROM {User} WHERE id=$1", postLast.user_id)[0];

		postLastXml = <>
			{postLast.ctime.ts}, {postUserLast.login}, <b>{postLast.map_title}</b>
		</>
	}

	topicsXml += <div>
	<table border="1">
		<tr>
			<th>
				
			</th>
			<th>
				Всего
			</th>
			<th>
				За неделю
			</th>
			<th>
				За сутки
			</th>
			<th>
				Последний
			</th>
		</tr>
		<tr>
			<th>
				Тем
			</th>
			<td>
				{topicsInf}
			</td>
			<td>
				{topics168}
			</td>
			<td>
				{topics24}
			</td>
			<td>
				{topicLastXml}
			</td>
		</tr>
		<tr>
			<th>
				Сообщений
			</th>
			<td>
				{postsInf}
			</td>
			<td>
				{posts168}
			</td>
			<td>
				{posts24}
			</td>
			<td>
				{postLastXml}
			</td>
		</tr>
	</table>
	</div>;

}

if(target.topics_navigate_date)
{
	topicsXml+=<>по датам за последние 10 дней</>;
	
	let dbr = orm.query('SELECT DISTINCT ctime::DATE FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL ORDER BY ctime DESC LIMIT 10', target.id);
	if(!dbr.length) dbr = [{ctime:new Date()}];
	let lastMonth = -1;
	let lastYear = (new Date()).getFullYear();
	for each(let r in dbr)
	{
		let d = r.ctime;
		let href = target.path+'/'+d.tsd;
		
		let name = [];
		if(lastYear != d.getFullYear())
		{
			name.push(d.getFullYear());
			lastYear = d.getFullYear();
		}
		if(lastMonth != d.getMonth())
		{
			name.push(['jan','feb','mar','apr','may','jun','jul','aug','sep','oct','nov','dec'][d.getMonth()]);
			lastMonth = d.getMonth();
		}
		name.push(d.getDate());
		topicsXml+=<a href={href}>[{name.join(' ')}]</a>;
	}
}

if(target.topics_navigate_page)
{
	topicsXml+=<><br/>по постоянным страницам последние 10 штук</>;
	
	let dbr = orm.query('SELECT DISTINCT page FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL AND page IS NOT NULL ORDER BY page DESC LIMIT 10', target.id);
	if(!dbr.length) dbr = [{page:1}];
	for each(let r in dbr)
	{
		let href = target.path+'/page'+r.page;
		topicsXml+=<a href={href}>[{r.page}]</a>;
	}
}

if(target.topics_navigate_rpage)
{
	topicsXml+=<><br/>по плывущим обратным страницам последние 10 штук</>;
	
	let dbr = orm.query('SELECT DISTINCT COUNT(*) FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL', target.id);
	let count = dbr[0].count || 1;
	let rpageSize = target.topics_navigate_rpage_size_ || 20;
	let rpages = Math.ceil(count / rpageSize)
	for(let rpage=1; rpage<=rpages; rpage++)
	{
		let href = target.path+'/rpage'+rpage;
		topicsXml+=<a href={href}>[{rpage}]</a>;
	}
}


return <div>
	Темы
	<div>{topicsXml}</div>
	<a href={forumPath+'/addTopic?backUrl='+forumPath+"/"+(new Date()).tsd}>Добавить тему</a>
</div>;