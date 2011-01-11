let data = arguments[0];

if('object' == typeof(data))
{
	if(data.src)
	{
		return ui.skin.tag('script', data.body||'', {type:"text/javascript", src:data.src});
	}
	return ui.skin.tag('script', data.body||'', {type:"text/javascript"});
}
return ui.skin.tag('script', '', {type:"text/javascript", src:data});
