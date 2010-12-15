let e = {};

///////////////////////////////////////////////
e.kind = undefined;

///////////////////////////////////////////////
//e.content = undefned;

///////////////////////////////////////////////
e.parser = 
{
	//kind:'*',
	//tags:{div:0},
	parse: function parse(xml, manager)
	{
		return null;
	},
};

///////////////////////////////////////////////
e.render = function render(mode)
{
	switch(mode)
	{
	case 'internal':
		return this.render_internal();
	case 'web':
		return this.render_web();
	case 'mail':
		return this.render_mail();
	case 'rss':
		return this.render_rss();
	};

	return this.render_web();
}

///////////////////////////////////////////////
e.render_internal = function render_internal()
{
	return 'super unable to render this instance: ' + dumps(this);
}

///////////////////////////////////////////////
e.render_web = function render_web()
{
	return 'super unable to render this instance: ' + dumps(this);
}

///////////////////////////////////////////////
e.render_mail = function render_mail()
{
	return 'super unable to render this instance: ' + dumps(this);
}

///////////////////////////////////////////////
e.render_rss = function render_rss()
{
	return 'super unable to render this instance: ' + dumps(this);
}

return e;