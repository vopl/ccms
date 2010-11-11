let target = arguments[0];
let point = this;

if(target.parent)
{
	let path = '';
	let iter = target.parent;
	while(iter.parent)
	{
		if(iter.map_path) path = iter.map_path + path;
		else path = iter.id + path;
		
		path = '/' + path;
		iter = iter.parent;
	}
	let href = point.parent.path+path;

	return <>
		<a href={href+'/'+target.id+'/edit?backUrl='+href+'/'+target.id+'/navigate'}> edit </a>
		<a href={href+'/'+target.id+'/del?backUrl='+href}> del </a>
		<a href={href+'/navigate'}> parent </a>
		<a href={href+'/'+target.id+'/add?backUrl='+href+'/'+target.id+'/navigate'}> add </a>
	</>;
}
else
{
	return <>
		<a href={point.parent.path+'/edit?backUrl='+request.path}> edit </a>
		<a href={point.parent.path+'/add?backUrl='+request.path}> add </a>
	</>;
}
return <></>;
