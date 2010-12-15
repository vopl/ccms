let e = {};

///////////////////////////////////////////////
e.kind = 'simpleTag';
e.tag = 'tag';
e.childs = [];

///////////////////////////////////////////////
e.parser = 
{
	kind:'element',
	//tags:{e.tag:0},
	tags:{},
	parse: function parse(xml, manager)
	{
		let res = {};
		res.__proto__ = e;
		//res.content = xml.toXMLString();

		let childs = xml.children();
		if(childs.length())
		{
			res.childs = manager.parse(childs);
			if(!(res.childs instanceof Array))
			{
				res.childs = [res.childs];
			}
		}
		return res;
	},
};

///////////////////////////////////////////////
e.render_internal = function render_internal()
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
		t.childs = this.childs.map(function(v) v.render_internal());
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
e.render_web = function render_web()
{
	return this.render_internal();
}

///////////////////////////////////////////////
e.render_mail = function render_mail()
{
	return this.render_internal();
}

///////////////////////////////////////////////
e.render_rss = function render_rss()
{
	return this.render_internal();
}

e.__proto__ = exec('super.js');
return e;