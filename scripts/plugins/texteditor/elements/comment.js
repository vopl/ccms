let e = {};

///////////////////////////////////////////////
e.kind = 'comment';

///////////////////////////////////////////////
e.content = '';

///////////////////////////////////////////////
e.template = router.createTemplate();
e.template.compileXml(<><!--{e.template.content}--></>);

///////////////////////////////////////////////
e.parser = 
{
	kind:'comment',
	parse: function parse(xml, context, manager)
	{
		let res = {};
		res.__proto__ = e;
		res.content = xml.toString();
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