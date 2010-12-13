let e = {};

///////////////////////////////////////////////
e.kind = undefined;

///////////////////////////////////////////////
//e.content = undefned;

///////////////////////////////////////////////
e.parser = 
{
	keys:
	[
		//{tag:'div', priority:0},
		//{list:'', priority:0},
	],
	parse: function parse(xml, context, manager)
	{
		return null;
	},
};

///////////////////////////////////////////////
e.render = function render(mode, context)
{
	switch(mode)
	{
	case 'internal':
		return this.render_internal(context);
	case 'web':
		return this.render_web(context);
	case 'mail':
		return this.render_mail(context);
	case 'rss':
		return this.render_rss(context);
	};

	return this.render_web(context);
}

///////////////////////////////////////////////
e.render_internal = function render_internal(context)
{
	return 'super unable to render this instance: ' + dumps(this);
}

///////////////////////////////////////////////
e.render_web = function render_web(context)
{
	return 'super unable to render this instance: ' + dumps(this);
}

///////////////////////////////////////////////
e.render_mail = function render_mail(context)
{
	return 'super unable to render this instance: ' + dumps(this);
}

///////////////////////////////////////////////
e.render_rss = function render_rss(context)
{
	return 'super unable to render this instance: ' + dumps(this);
}

return e;