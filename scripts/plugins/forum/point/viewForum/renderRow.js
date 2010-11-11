let target = arguments[0];
let point = this;

let path = '';
let iter = target;
while(iter.parent)
{
	if(iter.map_path) path = iter.map_path + path;
	else path = iter.id + path;
	
	path = '/' + path;
	iter = iter.parent;
}
let href = point.parent.path+path;

let res = <>
	</>;

if(target.allow_topic)
{
	res += 	<b><a href={href}>go {target.map_title}</a></b>;
}
res += 	<a href={href+'/navigate'}>navigate {target.map_title}</a>;
if(target.description)
{
	res += <p>{target.description}</p>;
}
	
res += <a href={href+'/edit?backUrl='+request.path}>[edit]</a>;
res += <a href={href+'/del?confirm=1&backUrl='+request.path}>[del]</a>;
res += <a href={href+'/add?backUrl='+request.path}>[add]</a>;

res += <a href={href+'/moveLeft?backUrl='+request.path}>[up]</a>;
res += <a href={href+'/moveRight?backUrl='+request.path}>[down]</a>;

if(target.childs.length)
{
	let childsList = <></>;
	for each(let cforum in target.childs)
	{
		childsList += this.properties.renderRow(cforum);
	}
	
	res += <ul>{childsList}</ul>;
}
//return <li><table border="1"><tr><td>{res}</td></tr></table></li>;
return <li>{res}</li>;