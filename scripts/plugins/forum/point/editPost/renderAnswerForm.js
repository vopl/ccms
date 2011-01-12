let id = arguments[0];
let pid = arguments[1];

let post;
if(id)
{
	post = orm.ForumPost.load({id:id});
}












let t = arguments.callee.t;

if(!t)
{
	t = router.createTemplate();

	t.compile(<>
		<form method="POST">
			<input type='hidden' name='save' value='1'/>

			map_title:<input type='string' name='map_title' value={t.map_title}/><br/>
			{t.content}
			<input type='button' onclick={'window.ccms.postTreeManager.doAnswer('+t.id+', '+t.pid+', true)'} value='Слать'/>
			<input type='button' onclick={'window.ccms.postTreeManager.doAnswer('+t.id+', '+t.pid+', false)'} value='Отмена'/>
		</form>

		</>);
	arguments.callee.t = t;
}
t = t.clone();


t.content = this.properties.te().renderNew('answerForm-'+pid, post?post.content:'');
t.id = id || "''";
t.pid = pid || "''";
t.map_title = post?post.map_title:'';

return t;