let pathPart = arguments[0];
let data = arguments[1];


let mostPost = data.posts[data.posts.length-1];

//////////////////////////////////////////////////
if(pathPart == 'edit')
{
	return {
		title:_('edit post $name', {$name:mostPost.map_title}),
		point:this.parent.childs.editPost,
	};
}

//////////////////////////////////////////////////
if(pathPart == 'del')
{
	return {
		point:this.parent.childs.delPost
	};
}

//////////////////////////////////////////////////
if(pathPart == 'add')
{
	data.mode = 'add';
	return {
		title:_('add post'),
		point:this.parent.childs.editPost,
	};
}

///////////////////////////////////////////////////
let dbr = undefined;
if(Math.round(pathPart)==pathPart)
{
	dbr = orm.query('SELECT * FROM {ForumPost} WHERE tree_pid=$1 AND id=$2', mostPost.id, pathPart);
}
if(!dbr || !dbr.length)
{
	dbr = orm.query('SELECT * FROM {ForumPost} WHERE tree_pid=$1 AND map_path=$2', mostPost.id, pathPart);
}

if(dbr.length)
{
	dbr = dbr[0];
	data.posts.push(dbr);

	let res = {
		title:dbr.map_title?dbr.map_title:('post #'+dbr.id),
		point:this,
	};
	
	return res;
}


warn('unknown path for '+absFile()+", '"+pathIn.join('/')+"'");
return false;