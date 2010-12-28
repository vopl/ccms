let target = arguments[0];
let point = this;




let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<div class='post'>

		<div class='forum-post-tree-structor' style='border-left: 1px solid #000; border-bottom: 1px solid #000;' level={t.level}>
			{t.user}: <b>{t.title}</b>
			{t.go} {t.edit} {t.del} {t.add}
			<div>{t.content}</div>
		</div>
		{t.childs}
	</div>);
	arguments.callee.t = t;
}
t = t.clone();

t.user = target.user.login;
t.title = target.title;

if(target.path != request.path)
{
	t.go = ui.skin.link(target.path, 'смотреть');
}

let parentPath = target.parent?target.parent.path:target.forum.path;
let postPathId = parentPath+'/'+target.id;
if(this.parent.childs.editPost.properties.access(target))
{
	t.edit = ui.skin.link(target.path+'/edit?backUrl='+postPathId, 'править');
}
if(this.parent.childs.delPost.properties.access(target))
{
	t.del = ui.skin.link(target.path+'/del?confirm=1&backUrl='+parentPath, 'удалить');
}

if(acl.hasRight('forum', 'writePost'))
{
	t.add = ui.skin.link(target.path+'/add?backUrl='+request.path, 'ответить');
}


t.content = {print:function()print(target.content), toString:function()target.content};

t.level = target.tree_level;
t.childs = target.childs?target.childs.map(function(v)point.properties.renderRow(v)):'';
	

return t;