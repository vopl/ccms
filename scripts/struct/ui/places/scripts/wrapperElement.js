let data = arguments[0];

if('object' == typeof(data))
{
	if(data.src)
	{
		return ui.skin.tag('script', data.body, {type:"text/javascript", src:data.src}, true);
	}
	return ui.skin.tag('script', data.body, {type:"text/javascript"}, true);
}
return ui.skin.tag('script', undefined, {type:"text/javascript", src:data}, true);
