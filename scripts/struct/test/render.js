let teman = router.plugins.texteditor.manager;
let te = teman.getTe(
	'myTeAtPath'+this.path, 
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


		onReset:function(did){warn('onReset')},
		onModify:function(did,doc){warn('onModify')},
		onSave:function(did,doc){warn('onSave')},
		onFinish:function(did){warn('onFinish')},
	});


let doc = exec('testdoc.js');


//let isid = teman.genIsid(te, 'myDid');
//let r = te.render(isid, doc);



let isid = teman.genIsid(te, 'myDid1');
teman.dropInstance(isid);
let instance = teman.getInstance(isid)
instance.doc = null;
instance.doc = doc;
let r = instance.render();

return r;
