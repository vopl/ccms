let target = arguments[0];
let point = this;

let path = '';
let iter = target;
while(iter)
{
	if(iter.map_path) path = iter.map_path + path;
	else path = iter.id + path;
	
	path = '/' + path;
	iter = iter.parent;
}

let href = point.parent.path;
for each(let forum in request.planData.forums)
{
	if(!forum.tree_pid) continue;
	href += '/';
	if(forum.map_path) href += forum.map_path;
	else href += forum.id;
}
href += path;

let res = <>
	</>;

if(target.map_title)
{
	res += <b>{target.map_title}</b>;
}
	
if(target.content)
{
	res += <b>{target.content}</b>;
}

	
res += <a href={href}>[go]</a>;
res += <a href={href+'/edit?backUrl='+request.path}>[edit]</a>;
res += <a href={href+'/del?confirm=1&backUrl='+request.path}>[del]</a>;
res += <a href={href+'/add?backUrl='+request.path}>[add]</a>;

if(target.childs.length)
{
	let childsList = <></>;
	for each(let cpost in target.childs)
	{
		childsList += this.properties.renderRow(cpost);
	}
	
	res += <ul>{childsList}</ul>;
}
//return <li><table border="1"><tr><td>{res}</td></tr></table></li>;
return <li>{res}</li>;