let pposts = request.planData.posts;
let targetPost = pposts.back;

let cposts = orm.query("SELECT * FROM {ForumPost} WHERE tree_root=$1 AND tree_left>$2 AND tree_right<$3 ORDER BY tree_left", targetPost.tree_root, targetPost.tree_left, targetPost.tree_right);
let posts = pposts.concat(cposts);
orm.ForumPost.linearToHierarchy(posts)[0];


let postsTree = [];

for each(post in targetPost.childs)
{
	postsTree.push(this.properties.renderRow(post));
}


let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	
	t.compile(<>
	заголовок <b>{t.title}</b><br/>
	содержимое <b>{t.content}</b><br/>
	<a href={t.editUrl}>редактировать</a><br/>
	<a href={t.delUrl}>удалить</a><br/>
	
	тут дерево дочерних постов
	{t.postsTree}
	
	<br/>
	<a href={t.newPostUrl}>новый пост</a><br/>


	<br/><br/><br/>
</>);
	
}
t = t.clone();

t.title = targetPost.map_title;
t.content = targetPost.content;
t.editUrl = request.path+'/edit?backUrl='+request.plan[request.plan.length-2].path+'/'+targetPost.id;
t.delUrl = request.path+'/del?backUrl='+request.plan[request.plan.length-2].path;
t.postsTree = postsTree;
t.newPostUrl = request.path+'/add?backUrl='+request.path;

return t;
