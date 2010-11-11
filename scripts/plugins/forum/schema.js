var orm = arguments[0];
var install = arguments[1];

if(!install)
{
	orm.delCategory('Forum');
	orm.delCategory('ForumPost');
	orm.delCategory('ForumPostAttachment');
	return;
}







///////////////////////////////////////////////////////////
orm.addCategory({
	name:'Forum',
	bases:['NsTree'],
	fields:
	{
		tree_pid:{type:'fkey',target:'Forum',},
		tree_root:{type:'fkey',target:'Forum',},

		map_path:'text',
		map_title:'text',
		
		allow_topic:'boolean',
		description:'string',
	},
});




///////////////////////////////////////////////////////////
orm.addCategory({
	name:'ForumPost',
	base:'NsTree',
	fields:
	{
		forum_id:{type:'fkey',target:'Forum',},
		map_path:'text',
		map_title:'text',
		tree_pid:{type:'fkey',target:'ForumPost',},
		tree_root:{type:'fkey',target:'ForumPost',},
		
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
