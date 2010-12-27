let pathPart = arguments[0];
let data = arguments[1];
let point = this;

//////////////////////////////////////////////////
//попробовать страницу
let pp = pathPart.match(/^page(\d+)$/);
if(pp)
{
	data.page = Number(pp[1]);
	
	return {
		title:_('page $num', {$num:data.page}),
		point:this.parent.childs.viewTopics,
	};
}

//////////////////////////////////////////////////
//попробовать обратную страницу
pp = pathPart.match(/^rpage(\d+)$/);
if(pp)
{
	data.rpage = Number(pp[1]);
	if(!data.rpage) return false;
	
	return {
		title:_('rpage $num', {$num:data.rpage}),
		point:this.parent.childs.viewTopics,
	};
}


//////////////////////////////////////////////////
//попробовать дату
pp = pathPart.match(/^(\d{4}-\d{2}-\d{2})$/);
if(pp)
{
	data.dateYMD = new Date();
	data.dateYMD.tsd = pp[1];
	
	return {
		title:_('date $date', {$date:data.dateYMD}),
		point:this.parent.childs.viewTopics,
	};
}

//////////////////////////////////////////////////
//редактирование
if(pathPart == 'edit')
{
	return {
		title:'Редактор',
		point:this.parent.childs.editForum,
	};
}

//////////////////////////////////////////////////
//изменение дерева - перемещение
if(pathPart == 'moveLeft')
{
	data.mode = 'moveLeft';
	return {
		point:this.parent.childs.moveForum,
	};
}

//////////////////////////////////////////////////
if(pathPart == 'moveRight')
{
	data.mode = 'moveRight';
	return {
		point:this.parent.childs.moveForum,
	};
}

//////////////////////////////////////////////////
//изменение дерева - удаление форума
if(pathPart == 'del')
{
	return {
		point:this.parent.childs.delForum,
	};
}

//////////////////////////////////////////////////
//изменение дерева - добавление дочернего форума
if(pathPart == 'add')
{
	data.mode = 'add';
	return {
		title:_('add forum'),
		point:this.parent.childs.editForum,
	};
}

//////////////////////////////////////////////////
//создание темы форума
if(pathPart == 'addTopic')
{
	data.mode = 'add';
	return {
		title:_('add topic'),
		point:this.parent.childs.editPost,
	};
}

//////////////////////////////////////////////////
//взять текущий форум
let mostForum = data.forum;
//дочерний
let forum;
//попробовать дочерний по id
if(Math.round(pathPart)==pathPart) forum = mostForum.childsById[pathPart];
//попробовать дочерний по map_path
if(!forum) forum = mostForum.childsByPath[pathPart];




if(forum)
{
	//дочерний найден
	data.forum = forum;

	return {
		title:forum.title,
		point:this,
	};
}

//дочерний не найден, попробовать тему
let topic = global.cache.process({
	key:'forum.'+mostForum.id+'.topic.'+pathPart,
	provider:function()
	{
		let topic;

		if(Math.round(pathPart)==pathPart)
			topic = orm.query('SELECT * FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL AND id=$2', mostForum.id, pathPart)[0];
		if(!topic)
			topic = orm.query('SELECT * FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL AND map_path=$2', mostForum.id, pathPart)[0];

		if(topic)
		{
			//дочерняя тема существует, выбрать для нее посты
			let posts = orm.query('SELECT * FROM {ForumPost} WHERE forum_id=$1  AND tree_root=$2 AND id!=$2 ORDER BY tree_left', mostForum.id, topic.id);
			
			let all = [topic].concat(posts);
			orm.NsTree.linearToHierarchy(all);
			
			let mkPath = point.parent.childs.viewPost.properties.mkPath;
			all.forEach(function(v){
				mkPath(v, mostForum);
				v.user = orm.query("SELECT * FROM {User} WHERE id=$1", v.user_id)[0];
			});
			
			return topic;
		}
		
		return false;
	},
	events:['forum.forum', 'forum.'+mostForum.id+'.topic', 'forum.'+mostForum.id+'.post'],
});


if(topic)
{
	data.topic = topic;
	data.post = topic;
	return {
		title:topic.title,
		point:this.parent.childs.viewPost,
	};
	
	return res;
}

warn('unknown path for '+absFile()+", '"+pathPart+"'");
return false;