let forum = request.planData.forum;
let topics;
if(request.planData.dateYMD)
{
	topics = orm.query("SELECT * FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 AND date_trunc('day', ctime)=date_trunc('day', $2::DATE) ORDER BY ctime DESC", forum.id, request.planData.dateYMD);
}
else if(request.planData.page)
{
	topics = orm.query("SELECT * FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 AND page=$2 ORDER BY ctime DESC", forum.id, request.planData.page);
}
else if(request.planData.rpage)
{
	let rpageSize = forum.topics_navigate_rpage_size || 10;
	topics = orm.query("SELECT * FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 ORDER BY ctime DESC LIMIT $2 OFFSET $3", forum.id, rpageSize, (request.planData.rpage-1)*rpageSize);
}

let table = <table border="1">
	<tr>
		<th>ctime</th>
		<th>title</th>
		<th>user</th>
		<th>stat</th>
	</tr>
</table>;

for each(let t in topics)
{
	let user = orm.query('SELECT * FROM {User} WHERE id=$1', t.user_id)[0];
	let stat1 = orm.query('SELECT COUNT(*) FROM {ForumPost} WHERE tree_root=$1 AND tree_pid IS NOT NULL', t.id)[0];
	let stat2 = orm.query('SELECT ctime FROM {ForumPost} WHERE tree_root=$1 AND tree_pid IS NOT NULL ORDER BY ctime DESC LIMIT 1', t.id)[0];
	
	table.tr += <tr>
		<td>{t.ctime}</td>
		<td><a href={forum.path+'/'+encodeURIComponent(t.map_path?t.map_path:t.id)}>{t.map_title}</a></td>
		<td><a href={this.parent.path+'/users/'+user.login}>{user.login}</a></td>
		<td>{user.login}</td>
		<td>ответов {stat1.count}, последний ответ {stat2?stat2.ctime:'отсутствует'}</td>
	</tr>;
}

return <>
	<a href={forum.path+'/addTopic?backUrl='+forum.path+"/"+(new Date()).tsd}>addTopic</a>
	таблица<br/>
	{table}
	навигатор на соседние<br/>
	навигатор на другой метод, страница которого содержит первый пост данного метода<br/>
	навигатор на форум<br/>
</>;