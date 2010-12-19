
return router.plugins.texteditor.manager.getTe(
	this.path, 
	{
		engine:'tmce',

		elements:
		{	
			//allow:['comment','bold', 'italic','strike','underline','insertion','deletion','subscript','superscript','paragraph'],
			//disallow:['comment','striked'],
		},

		facilities:
		{
			allow:['source','preview','save','cancel'],
			disallow:[],
                },
	}
);
