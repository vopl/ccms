let data = arguments[0];
let point = this;

if(!data.forum)
{
	data.forum = global.cache.process({
		key:'forum.root',
		provider:function()
		{
			//взять корень групп, если нет то создать
			let dbr = orm.query('SELECT * FROM {Forum} WHERE tree_pid IS NULL LIMIT 2');
			switch(dbr.length)
			{
			case 0:
				//корня нет, создать
				warn('root absent in Forum, try to insert');
				let root = orm.Forum.insert({});
				dbr = orm.query('SELECT * FROM {Forum} WHERE tree_pid IS NULL');
				break;
			case 1:
				//корень один, нормальная штатная ситуация
				//warn('root in Forum ok');
				break;
			case 2:
				//корней больше чем один, снести лишние
				warn('multiple roots in Forum, try to delete');
				orm.exec('DELETE FROM {Forum} WHERE tree_pid IS NULL AND id!=$1', dbr[0].id);
				dbr = orm.query('SELECT * FROM {Forum} WHERE tree_pid IS NULL');
				break;
			}
			let root = dbr[0];

			let all = orm.query('SELECT * FROM {Forum} WHERE tree_root=$1 AND id!=$1', root.id);
			all.push(root);

			orm.NsTree.linearToHierarchy(all);
			all.forEach(function(v) point.childs.viewForum.properties.mkPath(v));

			return root;

		},
		events:['forum.forum'],
	});
}