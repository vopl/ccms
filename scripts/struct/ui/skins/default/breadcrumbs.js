let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<div class="breadcrumbs">{t.arr}</div>);
	arguments.callee.t = t;
}

t = t.clone();
t.arr = [];

let src = arguments[0];
for(let idx=0; idx<src.length; idx++)
{
	if(idx < src.length-1)
	{
		t.arr.push(this.properties.link(src[idx].href, src[idx].title));
		t.arr.push(<> &#187;&#160;</>);
	}
	else
	{
		t.arr.push(src[idx].title);
	}
}

return t;