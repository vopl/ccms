let target = arguments[0];
let point = this;




let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(
	<div id={'forum-post-'+t.id} pid={t.pid} fid={t.fid} rev={t.revision} class={t.class}>
		<div level={t.level} class='forum-post-structor'>
			{t.user}: <b>{t.title}</b>
			{t.go} {t.edit} {t.del} {t.add}
			<div id={'forum-post-content-'+t.id} class='forum-post-content'>{t.content}</div>
		</div>
		<div class='forum-post-childs'>
			{t.childs}
		</div>
	</div>);
	arguments.callee.t = t;
}
t = t.clone();

t.id = target.id;
t.pid = target.tree_pid?target.tree_pid:'';
t.fid = target.forum_id;
t.revision = target.revision;
t.class = 'forum-post';
if(!target.tree_pid) t.class += ' forum-topic';
t.level = target.tree_level;

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

t.childs = target.childs?target.childs.map(function(v)point.properties.renderRow(v)):'';
	

return t;