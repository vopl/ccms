let pathPart = arguments[0];
let data = arguments[1];


//////////////////////////////////////////////////
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
let mostForum = data.forum;

//////////////////////////////////////////////////
if(pathPart == 'edit')
{
	return {
		title:'Редактор',
		point:this.parent.childs.editForum,
	};
}

//////////////////////////////////////////////////
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
if(pathPart == 'del')
{
	return {
		point:this.parent.childs.delForum,
	};
}

//////////////////////////////////////////////////
if(pathPart == 'add')
{
	data.mode = 'add';
	return {
		title:_('add forum'),
		point:this.parent.childs.editForum,
	};
}

//////////////////////////////////////////////////
if(pathPart == 'addTopic')
{
	data.mode = 'add';
	return {
		title:_('add topic'),
		point:this.parent.childs.editPost,
	};
}

//////////////////////////////////////////////////
let forum = mostForum.childsByPath[pathPart];
if(!forum) forum = mostForum.childsById[pathPart];


if(forum)
{
	data.forum = forum;

	return {
		title:forum.title,
		point:this,
	};
}


//////////////////////////////////////////////////
let dbr = undefined;
if(Math.round(pathPart)==pathPart)
{
	dbr = orm.query('SELECT * FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL AND id=$2', mostForum.id, pathPart);
}
if(!dbr || !dbr.length)
{
	dbr = orm.query('SELECT * FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL AND map_path=$2', mostForum.id, pathPart);
}

if(dbr.length)
{
	dbr = dbr[0];
	if(!data.posts) data.posts = [];
	data.posts.push(dbr);

	let res = {
		title:dbr.map_title?dbr.map_title:('post #'+dbr.id),
		point:this.parent.childs.viewPost,
	};
	
	return res;
}

warn('unknown path for '+absFile()+", '"+pathPart+"'");
return false;