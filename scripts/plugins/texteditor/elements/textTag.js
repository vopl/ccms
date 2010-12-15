let e = {};

///////////////////////////////////////////////
e.kind = 'textTag';

///////////////////////////////////////////////
e.content = '';

///////////////////////////////////////////////
e.template = router.createTemplate();
e.template.compileText(e.template.content);

///////////////////////////////////////////////
e.parser = 
{
	kind:'element',
	tags:{'*':-10},
	parse: function parse(xml, manager)
	{
		let res = {};
		res.__proto__ = e;
		res.content = esc.xml(xml.toXMLString());
		return res;
	},
};

///////////////////////////////////////////////
e.render_internal = function render_internal()
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

///////////////////////////////////////////////
e.render_web = function render_web()
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

///////////////////////////////////////////////
e.render_mail = function render_mail()
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

///////////////////////////////////////////////
e.render_rss = function render_rss()
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

e.__proto__ = exec('super.js');
return e;