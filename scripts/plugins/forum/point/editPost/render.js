let post = arguments[0];












let t = arguments.callee.t;

if(!t)
{
	t = router.createTemplate();

	t.compile(<>
		<form method="POST">
			<input type='hidden' name='save' value='1'/>
			<input type='hidden' name='id' value={t.id}/>

			map_title:<input type='string' name='map_title' value={t.map_title}/><br/>
			{t.content}
			<input type='submit'/>
		</form>

		</>);
	arguments.callee.t = t;
}
t = t.clone();


t.content = this.properties.te().renderNew(post.id, post.content);
t.id = post.id;
t.map_title = post.map_title;
t.map_path = post.map_path;

return t;