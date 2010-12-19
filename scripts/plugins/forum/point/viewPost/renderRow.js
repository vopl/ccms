let target = arguments[0];
let point = this;

let path = '';
let iter = target;
while(iter)
{
	if(iter.map_path) path = iter.map_path + path;
	else path = iter.id + path;
	
	path = '/' + path;
	iter = iter.parent;
}

let href = point.parent.path;
for each(let forum in request.planData.forums)
{
	if(!forum.tree_pid) continue;
	href += '/';
	if(forum.map_path) href += forum.map_path;
	else href += forum.id;
}
href += path;







let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<li>
		<b>{t.title}</b>
		{t.go}
		{t.edit}
		{t.del}
		{t.add}
		{t.content}
		<hr/>
		{t.childs}
	</li>);
	arguments.callee.t = t;
}
t = t.clone();

t.title = target.map_title;
t.content = new XML('<div>'+target.content+'</div>');

t.go = ui.skin.link(href, 'ходи');
t.edit = ui.skin.link(href+'/edit?backUrl='+request.path, 'редактировать');
t.del = ui.skin.link(href+'/del?confirm=1&backUrl='+request.path, 'удалить');
t.add = ui.skin.link(href+'/add?backUrl='+request.path, 'ответить');

	
if(target.childs.length)
{
	t.childs = [];
	let tc = arguments.callee.tc;
	if(!tc)
	{
		tc = router.createTemplate();
		tc.compile(<ul>{tc.content}</ul>);
		arguments.callee.tc = tc;
	}

	for each(let cpost in target.childs)
	{
		let tcc = tc.clone();
		tcc.content = this.properties.renderRow(cpost);
		t.childs.push(tcc);
	}
}

return t;