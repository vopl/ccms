let target = arguments[0];
let point = this;

let href = target.path;

let res = <>
	</>;

res += 	<b><a href={href}>go {target.map_title}</a></b>;


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
		childsList += this.properties.renderNavigateRow(cforum);
	}
	
	res += <ul>{childsList}</ul>;
}
//return <li><table border="1"><tr><td>{res}</td></tr></table></li>;
return <li>{res}</li>;