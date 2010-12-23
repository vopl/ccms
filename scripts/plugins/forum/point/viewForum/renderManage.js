let target = arguments[0];
if(!target) target = request.planData.forum;
let point = this;
let parentForum = target.parent;

let res = [];

//редактировать
//удалить
//двинуть вверх
//двинуть вниз
if(acl.hasRight('forum', 'manageForums'))
{
	if(parentForum)
	{
		res.push(ui.skin.tag('a', '[ред]', {href:target.path+'/edit?backUrl='+parentForum.path+'/'+target.id}));
		res.push(ui.skin.tag('a', '[нах]', {href:target.path+'/del?backUrl='+parentForum.path}));
	}
	else
	{
		res.push(ui.skin.tag('a', '[ред]', {href:target.path+'/edit?backUrl='+request.path}));
	}

	res.push(ui.skin.tag('a', '[добавить]', {href:target.path+'/add?backUrl='+request.path}));

	if(request.path != target.path)
	{
		res.push(ui.skin.tag('a', '[вверх]', {href:target.path+'/moveLeft?backUrl='+request.path}));
		res.push(ui.skin.tag('a', '[вниз]', {href:target.path+'/moveRight?backUrl='+request.path}));
	}
}
return res;