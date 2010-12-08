var orm = arguments[0];
var install = arguments[1];

if(!install)
{
	orm.delCategory('Texteditor');
	orm.delCategory('TexteditorImages');
	orm.delCategory('TexteditorFiles');
	return;
}



///////////////////////////////////////////////////////////
orm.addCategory({
	name:'Texteditor',
	inherits:'BaseCategory',
	fields:
	{
		ctime:'date',
		mtime:'date',

		sid:'string',
		uid:'string',

		options:'string',
		content:'text',
	},
	
});


///////////////////////////////////////////////////////////
orm.addCategory({
	name:'TexteditorImages',
	fields:
	{
		texteditor_id:{type:'fkey', target:'Texteditor'},
		image_id:{type:'fkey', target:'Image'},
	},
});


///////////////////////////////////////////////////////////
orm.addCategory({
	name:'TexteditorFiles',
	fields:
	{
		texteditor_id:{type:'fkey', target:'Texteditor'},
		file_id:{type:'fkey', target:'File'},
	},
});
