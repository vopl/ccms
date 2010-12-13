let e = {};

///////////////////////////////////////////////
e.kind = 'text';

///////////////////////////////////////////////
e.content = '';

///////////////////////////////////////////////
e.template = router.createTemplate();
e.template.compileText(e.template.content);

///////////////////////////////////////////////
e.parser = 
{
	keys:
	[
		{tag:'*', priority:-10},
		{list:'', priority:-10},
	],
	parse: function parse(xml, context, manager)
	{
		let res = {};
		res.__proto__ = e;
		res.content = xml.toXMLString();
		return res;
	},
};

///////////////////////////////////////////////
e.render_internal = function render_internal(context)
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

///////////////////////////////////////////////
e.render_web = function render_web(context)
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

///////////////////////////////////////////////
e.render_mail = function render_mail(context)
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

///////////////////////////////////////////////
e.render_rss = function render_rss(context)
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

e.__proto__ = exec('super.js');
return e;