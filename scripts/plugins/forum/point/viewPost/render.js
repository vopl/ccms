ui.blocks.scripts.jquery144='/js/jquery-1.4.4.js';

let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<>{t.man}<div class='forum-posts'>{t.posts}</div></>);
	arguments.callee.t = t;
}
t = t.clone();
t.man = this.properties.renderTreeMan();
t.posts = this.properties.renderRow(request.planData.post);


ui.blocks.scriptsTail.postTreeManager = this.parent.path+'/postTreeManager.js';
ui.blocks.scriptsTail.postTreeManagerInit = {body:<>window.ccms.postTreeManager.init("{esc.str(request.planData.post.path)}");</>};

return t;
