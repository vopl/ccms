let pathPart = arguments[0];
let data = arguments[1];


let mostPost = data.posts[data.posts.length-1];

//резолвить на топик


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


warn('unknown path for '+absFile()+", '"+pathPart+"'");
return false;