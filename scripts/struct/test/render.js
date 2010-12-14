let teman = router.plugins.texteditor.manager;
let te = teman.getTe(
	this.path, 
	{
		engine:'tmce',
		
		allow:['bold','comment','deletion','insertion','italic','strike','subscript','superscript','text','underline',],
		disallow:['comment','strike'],

		onReset:function(did){warn('onReset')},
		onModify:function(did,doc){warn('onModify')},
		onSave:function(did,doc){warn('onSave')},
		onCancel:function(did){warn('onCancel')},
	});


let doc = 
{
	dom:
		<>
			text content
			<b>bold</b>
			text content
			<b><em>bold italic</em></b>
			text content
		</>,
	data:{},
}

let isid = teman.genIsid(te, 'myDid');

let r = te.render(isid, doc);

return r;
