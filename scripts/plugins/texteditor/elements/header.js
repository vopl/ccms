let e = {};

///////////////////////////////////////////////
e.kind = 'header';
e.childs = [];

///////////////////////////////////////////////
e.parser = 
{
	kind:'element',
	tags:
	{
		h1:0,
		h2:0,
		h3:0,
		h4:0,
		h5:0,
		h6:0,
	},
	parse: function parse(xml, manager)
	{
		let res = {};
		res.__proto__ = e;

		switch(String(xml.name()))
		{
		case 'h1': res.level=1;break;
		case 'h2': res.level=2;break;
		case 'h3': res.level=3;break;
		case 'h4': res.level=4;break;
		case 'h5': res.level=5;break;
		case 'h6': res.level=6;break;
		default: return null;
		}

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

e.templates = 
{
	internal:router.createTemplate(),
	web:router.createTemplate(),
};

e.templates.internal.compile(<{'h'+e.templates.internal.level}>{e.templates.internal.childs}</{'h'+e.templates.internal.level}>);
e.templates.web = e.templates.internal;
e.templates.mail = e.templates.internal;
e.templates.rss = e.templates.internal;

///////////////////////////////////////////////
///////////////////////////////////////////////
e.render = function render(mode)
{
	let t = e.templates[mode].clone();

	t.level = this.level;
	t.childs = this.childs.map(function(v) v.render(mode));
	return t;
}


e.__proto__ = exec('super.js');
return e;