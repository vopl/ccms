function mkMap(map, path)
{
	if(!map) return <></>;
	
	let res = <></>;
	let title = map.title;
	if(!title && map.point.properties.title) title = map.point.properties.title();
	
	res += <a href={path}>{title?title:path}</a>;
	
	if(map.childs)
	{
		let childs = <></>;
		
		for(let cname in map.childs)
		{
			childs += mkMap(map.childs[cname], path+cname+'/');
		}
		
		res += <ul>{childs}</ul>;
	}
	
	return <li>{res}</li>;
}

return	<div>
	{(function(){
		if(global.user.login == 'anonymous')
		{
			return <>там слева можно авторизоваться</>
		}
		return "";
	})()}<br/>
	
	<b>
		<ul>
			{mkMap(this.map({childs:1, forum:1, forumTopic:1}), '/')}
		</ul>
	</b>
	
	<pre>
		{dumps(this.map({childs:1, forum:1, forumTopic:1}), 2)};
	</pre>
</div>
