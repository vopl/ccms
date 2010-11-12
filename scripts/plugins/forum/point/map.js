let options = arguments[0];

if(!acl.hasRight('forum', 'view'))
{
	return null;
}

let res = {
	point:this,
	title:this.properties.title(),
	childs:{},
};

if(options.forum)
{
	let dbr = orm.query('SELECT id, tree_pid, map_title, map_path FROM {Forum} WHERE tree_pid IS NOT NULL ORDER BY tree_left');
	let fbid = {};
	let froots = [];
	for each(let forum in dbr) 
	{
		fbid[forum.id] = forum; forum.childs=[]
	};
	for each(let forum in dbr)
	{
		forum.parent = fbid[forum.tree_pid];
		if(forum.parent)
		{
			forum.parent.childs.push(forum);
		}
		else
		{
			froots.push(forum);
		}
	};
	
	function mkOne(forums)
	{
		let res = {};
		for each(let forum in forums)
		{
			res[forum.map_path?forum.map_path:forum.id] = 
			{
				title:forum.map_title?forum.map_title:('forum #'+forum.id),
				//point:this,
				childs:forum.childs?mkOne(forum.childs):{},
			};
		}
		
		return res;
	}
	res.childs = mkOne(froots);
}

return res;
