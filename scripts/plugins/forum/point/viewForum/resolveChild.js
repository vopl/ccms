let pathPart = arguments[0];
let data = arguments[1];



//////////////////////////////////////////////////
let pp = pathPart.match(/^page(\d+)$/);
if(pp)
{
	data.page = pp[1];
	data.nonPagePath = '/'+pathOut.join('/');
	
	return {
		title:_('page $num', {$num:data.page}),
		point:this,
	};
}


//////////////////////////////////////////////////
let mostForum = data.forums.back;

//////////////////////////////////////////////////
if(pathPart == 'edit')
{
	return {
		title:_('edit forum $name', {$name:mostForum.map_title}),
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
		title:_('add forum'),
		point:this.parent.childs.editPost,
	};
}

//////////////////////////////////////////////////
let point = this;
let forum = global.cache.process({
	key:'forum.child.'+mostForum.id+'.'+pathPart,
	provider:function()
	{
		let dbr;
		if(Math.round(pathPart)==pathPart)
		{
			dbr = orm.query('SELECT * FROM {Forum} WHERE tree_pid=$1 AND id=$2', mostForum.id, pathPart);
		}
		if(!dbr || !dbr.length)
		{
			dbr = orm.query('SELECT * FROM {Forum} WHERE tree_pid=$1 AND map_path=$2', mostForum.id, pathPart);
		}
		if(dbr.length)
		{
			dbr = dbr[0];
			dbr.parent = data.forums.back;
			point.properties.mkPath(dbr);
			return dbr;
		}
		return null;
	},
	events: ['forum.forum'],
});


if(forum)
{
	data.forums.push(forum);

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