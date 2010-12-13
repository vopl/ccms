let e = {};

///////////////////////////////////////////////
e.kind = 'simpleTag';
e.tag = 'tag';

///////////////////////////////////////////////
e.parser = 
{
	keys:
	[
		{tag:e.tag, priority:0},
	],
	parse: function parse(xml, context, manager)
	{
		let res = {};
		res.__proto__ = e;
		res.content = xml.toXMLString();

		let childs = xml.children();
		if(childs.length())
		{
			res.childs = manager.parse(childs);
		}
		return res;
	},
};

///////////////////////////////////////////////
e.render_internal = function render_internal(context)
{
	if('childs' in this)
	{
		let t = e.render_internal.t;
		if(!t)
		{
			t = router.createTemplate();
			t.compile(<{e.tag}>{t.childs}</{e.tag}>);
			e.render_internal.t = t;
		}
		t = t.clone();
		t.childs = this.childs.map(function(v) v.render_internal(context));
		return t;
	}

	let te = e.render_internal.te;
	if(!te)
	{
		te = router.createTemplate();
		te.compile(<{e.tag}/>);
		e.render_internal.te = te;
	}
	return te;
}

///////////////////////////////////////////////
e.render_web = function render_web(context)
{
	return this.render_internal(context);
}

///////////////////////////////////////////////
e.render_mail = function render_mail(context)
{
	return this.render_internal(context);
}

///////////////////////////////////////////////
e.render_rss = function render_rss(context)
{
	return this.render_internal(context);
}

e.__proto__ = exec('super.js');
return e;