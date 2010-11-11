let pposts = request.planData.posts;
let targetPost = pposts[pposts.length-1];

let cposts = orm.query("SELECT * FROM {ForumPost} WHERE tree_root=$1 AND tree_left>$2 AND tree_right<$3 ORDER BY tree_left", targetPost.tree_root, targetPost.tree_left, targetPost.tree_right);
let posts = pposts.concat(cposts);
orm.ForumPost.linearToHierarchy(posts)[0];


let postsTree = <></>;

for each(post in targetPost.childs)
{
	postsTree += this.properties.renderRow(post);
}

return <>
	заголовок <b>{targetPost.map_title}</b><br/>
	содержимое <b>{targetPost.content}</b><br/>
	<a href={request.path+'/edit?backUrl='+request.plan[request.plan.length-2].path+'/'+targetPost.id}>редактировать</a><br/>
	<a href={request.path+'/del?backUrl='+request.plan[request.plan.length-2].path}>удалить</a><br/>
	
	тут дерево дочерних постов
	{postsTree}
	
	<br/>
	<a href={request.path+'/add?backUrl='+request.path}>новый пост</a><br/>


	<br/><br/><br/>

	<pre>
	dumps(request.planData.topic, 3)}
	{dumps(request.plan, 3)}
	</pre>
</>;
