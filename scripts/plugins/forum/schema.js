var orm = arguments[0];
var install = arguments[1];

if(!install)
{
	orm.delCategory('ForumMaped');
	orm.delCategory('Forum');
	orm.delCategory('ForumImage');
	orm.delCategory('ForumPost');
	orm.delCategory('ForumPostImage');
	orm.delCategory('ForumPostFile');
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
	
	methods:
	{
		adoptMapPath: function adoptMapPath()
		{
			let trans = arguments.callee.trans;
			if(!trans)
			{
				trans =  new Icu.Transliterator('Any-Latin; any-NFD; Any-Publishing; [:^ascii:]Any-Remove');
				arguments.callee.trans = trans;
			}
			this.map_path = trans.transliterate(this.map_path);

			//this.map_path = this.map_path.replace(/[\x00-\x1f\x7f\!\@\#\$\%\^\&\*\(\)\|\`\<\>\?\/\\\:\;\'\:\"\s]+/g, ' ');
			//this.map_path = this.map_path.replace(/\s+/g, '-');
			this.map_path = this.map_path.replace(/[\s\W]+/g, '-');
		},
/*
		incrementMapPath: function incrementMapPath()
		{
			let v = this.map_path;
			v = v.split('').reverse().join('');
			let expr = /.*?(\d+)/g;
			expr.lastIndex=0;

			let arr = expr.exec(v);
			if(arr)
			{
				arr = arr[1].split('').reverse().join('');
				arr = String(Number(arr)+1);
				arr = arr.split('').reverse().join('');
				v = v.substring(0, expr.lastIndex-2) + arr + v.substring(expr.lastIndex, 999999999);
			}
			else
			{
				v = "1" + v;
			}

			v = v.split('').reverse().join('');
			
			this.map_path = v;
			return v;
		},
*/
		incrementMapPath: function incrementMapPath()
		{
			this.map_path = String(this.map_path) + Crypto.Rand.str_(1);
			return this.map_path;
		},

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

		description:'string',

		topic_allow:{type:'boolean', default:true,},
		topics_navigate_date:{type:'boolean', default:true,},
		topics_navigate_page:{type:'boolean', default:true,},
		topics_navigate_rpage:{type:'boolean', default:true,},
		topics_navigate_page_size:{type:'integer', default:20,},
		topics_navigate_rpage_size:{type:'integer', default:20,},

		image_id:{type:'fkey',target:'Image', onUpdate:'setNull', onDelete:'setNull'},
	},
});


///////////////////////////////////////////////////////////
orm.addCategory({
	name:'ForumImage',
	fields:
	{
		forum_id:{type:'fkey',target:'Forum',},
		image_id:{type:'fkey',target:'Image',},
	}
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

		user_id:{type:'fkey',target:'User',notNull:true},
		
		ctime:{type:'date',notNull:true},
		mtime:{type:'date',notNull:true},
		
		//отношение к экземпляру форума
		forum_id:{type:'fkey',target:'Forum',notNull:true,},
		
		//отношение к "постоянной странице"
		page:{type:'integer',notNull:true},
		
		content:{type:'text',notNull:true},
	},
});

///////////////////////////////////////////////////////////
orm.addCategory({
	name:'ForumPostImage',
	fields:
	{
		forum_id:{type:'fkey',target:'Forum',},
		image_id:{type:'fkey',target:'Image',},
		kind:'string',//tiny, small, normal
	}
});

///////////////////////////////////////////////////////////
orm.addCategory({
	name:'ForumPostFile',
	fields:
	{
		forum_id:{type:'fkey',target:'Forum',},
		file_id:{type:'fkey',target:'File',},
	}
});