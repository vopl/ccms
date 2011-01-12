let mostForum = request.planData.forum;
let mostPost = request.planData.post;

let mode = request.params.mode || '';
{
	switch(mode)
	{
	case 'answerForm':
		request.pushHeader('Content-Type', "text/xml; charset=utf-8");
		print('<?xml version="1.0" encoding="utf-8"?>\n');
		print('<div class="forum-post-answer-form">');
			this.properties.renderAnswerForm(request.params.id||undefined, request.params.pid||undefined).print();
		print('</div>');
		return;
	case 'answer':
		if(request.params.pid)
		{
			request.planData.mode = 'add';
		}
		mostPost = orm.ForumPost.load({id:request.params.id || request.params.pid});
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////
if(request.params.save)
{
	let post;

	if(request.planData.mode && request.planData.mode == 'add')
	{
		post = orm.ForumPost.make(request.params);
		post.id = undefined;

		let dbr = orm.query('SELECT MAX(page) FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL', mostForum.id);
		let mostPage = dbr[0].max || 1;
		dbr = orm.query('SELECT COUNT(*) FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL AND page=$2', mostForum.id, mostPage);
		let mostPageSize = dbr[0].count || 0;

		if(mostPageSize >= (mostForum.topics_navigate_page_size || 20))
		{
			mostPage++;
		}

		post.page = mostPage;
		post.tree_pid = mostPost?mostPost.id:undefined;
		post.ctime = new Date();
		post.user_id = user.id;
	}
	else
	{
		post = orm.ForumPost.load({id:mostPost.id});
		post.set(request.params);

		post.id = mostPost.id;
		post.tree_pid = mostPost.tree_pid;
		post.map_path = request.params.map_path;
	}

	if(!post.map_title) post.map_title = mostPost?mostPost.map_title:'штука без названия';
	if(!post.map_path) post.map_path = post.map_title;

	if(!post.content) post.content = " ";

	post.content = this.properties.te().renderDoc(post.content);
	post.forum_id = mostForum.id;
	post.mtime = new Date();

	post.adoptMapPath();

	for(;;)
	{
		let dbr;
		if(post.tree_pid)
		{
			if(post.id)
			{
				//обновление с родителем (обновлние ответа)
				dbr = orm.query('SELECT id FROM {ForumPost} WHERE map_path=$1 AND forum_id=$2 AND tree_pid=$3 AND id!=$4', post.map_path, mostForum.id, post.tree_pid, post.id);
			}
			else
			{
				//вставка с родителем (создание ответа)
				dbr = orm.query('SELECT id FROM {ForumPost} WHERE map_path=$1 AND forum_id=$2 AND tree_pid=$3', post.map_path, mostForum.id, post.tree_pid);
			}
		}
		else
		{
			if(post.id)
			{
				//обновление без родителя (обновлние темы)
				dbr = orm.query('SELECT id FROM {ForumPost} WHERE map_path=$1 AND forum_id=$2 AND tree_pid IS NULL AND id!=$3', post.map_path, mostForum.id, post.id);
			}
			else
			{
				//вставка без родителя (создание темы)
				dbr = orm.query('SELECT id FROM {ForumPost} WHERE map_path=$1 AND forum_id=$2 AND tree_pid IS NULL', post.map_path, mostForum.id);
			}
		}
		if(!dbr.length) break;

		post.incrementMapPath();
	}

	post.save();

	cache.fire('forum.'+request.planData.forum.id+'.post');
	if(!post.tree_pid)
	{
		cache.fire('forum.'+request.planData.forum.id+'.topic');
	}


	switch(mode)
	{
	case 'answer':
		request.pushHeader('Content-Type', "text/xml; charset=utf-8");
		print('<?xml version="1.0" encoding="utf-8"?>\n');
		print('<ok/>');
		return;
	default:
		break;
	}


	request.setStatusCode(303);
	let ppath = request.path.substr(0, request.path.lastIndexOf('/'));
	request.pushHeader('Location', request.params.backUrl?request.params.backUrl:ppath);
	return;

}
else if(request.planData.mode && request.planData.mode == 'add')
{
	ui.blocks.center.push(this.render({map_title:mostPost?mostPost.map_title:''}));
	ui.print();
}
else
{
	ui.blocks.center.push(this.render(mostPost));
	ui.print();
}

