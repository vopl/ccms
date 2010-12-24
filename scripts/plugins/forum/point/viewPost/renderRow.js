let target = arguments[0];
let point = this;




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

t.title = target.title;
t.content = new XML('<div>'+target.content+'</div>');

t.go = ui.skin.link(target.path, 'ходи');
t.edit = ui.skin.link(target.path+'/edit?backUrl='+request.path, 'редактировать');
t.del = ui.skin.link(target.path+'/del?confirm=1&backUrl='+request.path, 'удалить');
t.add = ui.skin.link(target.path+'/add?backUrl='+request.path, 'ответить');

	
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