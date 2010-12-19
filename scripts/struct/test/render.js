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


		onReset:function(inst){warn('onReset my')},
		onSave:function(inst){warn('onSave my' + inst.te.i2e(inst.doc))},
		onFinish:function(inst){warn('onFinish my')},
	});




//let isid = teman.genIsid(te, 'myDid');
//let r = te.render(isid, doc);



let isid = teman.genIsid(te, 'myDid1');
let instance = teman.getInstance(isid);
if(!instance.doc)
{
	warn('load doc');
	instance.doc = exec('testdoc.js');
}
let r = instance.render();

return r;
