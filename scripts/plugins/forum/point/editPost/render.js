let post = arguments[0];


return <>
<form method="POST">
	<input type='hidden' name='save' value='1'/>
	<input type='hidden' name='id' value={post.id?post.id:''}/>

	map_title:<input type='string' name='map_title' value={post.map_title?post.map_title:''}/><br/>
	map_path:<input type='string' name='map_path' value={post.map_path?post.map_path:''}/><br/>
	content:<textarea name='content'>{post.content?post.content:''}</textarea><br/>
	<input type='submit'/>
</form>

<pre>{dumps(request.plan, 2)}</pre>;
</>
