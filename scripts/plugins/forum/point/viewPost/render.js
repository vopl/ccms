let targetPost = request.planData.post;
let point = this;

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
t.postsTree = targetPost.childs.map(function(v) point.properties.renderRow(v));
t.newPostUrl = request.path+'/add?backUrl='+request.path;

return t;
