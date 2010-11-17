var orm = arguments[0];
var install = arguments[1];

if(!install)
{
	orm.delCategory('ForumMaped');
	orm.delCategory('Forum');
	orm.delCategory('ForumPost');
	orm.delCategory('ForumPostAttachment');
	return;
}








///////////////////////////////////////////////////////////
orm.addCategory({
	name:'ForumMapped',
	abstract:true,
	fields:
	{
		//для проекции на урлы часть пути
		map_path:'text',
		//и некий заголовок
		map_title:'text',
	},
});






///////////////////////////////////////////////////////////
orm.addCategory({
	name:'Forum',
	bases:['NsTree', 'ForumMapped'],
	fields:
	{
		//дерево внутри форумов
		tree_pid:{type:'fkey',target:'Forum',},
		tree_root:{type:'fkey',target:'Forum',},

		allow_topic:'boolean',
		description:'string',
	},
});




///////////////////////////////////////////////////////////
orm.addCategory({
	name:'ForumPost',
	bases:['NsTree', 'ForumMapped'],
	fields:
	{
		//дерево внутри постов. Корень - тема которая тоже пост
		tree_pid:{type:'fkey',target:'ForumPost',},
		tree_root:{type:'fkey',target:'ForumPost',},
	
		//отношение к экземпляру форума
		forum_id:{type:'fkey',target:'Forum',},
		//отношение к "постоянной странице"
		ppage:'integer',
		
		content:'text',
	},
});

///////////////////////////////////////////////////////////
orm.addCategory({
	name:'ForumPostAttachment',
	base:'BaseCategory',
	fields:
	{
		post_id:{type:'fkey',target:'ForumPost',},
		server_name:'string',
		client_name:'string',
		size:'integer',
		comment:'string',
	},
});
