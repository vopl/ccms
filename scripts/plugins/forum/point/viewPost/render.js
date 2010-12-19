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
	<b>{t.title}</b>
	<a href={t.editUrl}>редактировать</a>
	<a href={t.delUrl}>удалить</a>
	<a href={t.newPostUrl}>ответить</a>

	{t.content}
	<hr/>
	
	<ul>{t.postsTree}</ul>
</>);
	
}
t = t.clone();

t.title = targetPost.map_title;
t.content = {print:function()print(targetPost.content), toString:function()targetPost.content};
t.editUrl = request.path+'/edit?backUrl='+request.plan[request.plan.length-2].path+'/'+targetPost.id;
t.delUrl = request.path+'/del?backUrl='+request.plan[request.plan.length-2].path;
t.postsTree = postsTree;
t.newPostUrl = request.path+'/add?backUrl='+request.path;

return t;
