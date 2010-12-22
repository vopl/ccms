let e = {};

///////////////////////////////////////////////
e.kind = 'simpleTag';
e.tag = 'tag';

///////////////////////////////////////////////
e.parser = 
{
	kind:'element',
	noChilds:false,
	tags:{},
	parse: function parse(xml, manager)
	{
		let res = {};
		res.__proto__ = e;

		if(!e.parser.noChilds)
		{
			let childs = xml.children();
			if(childs.length())
			{
				res.childs = manager.parse(childs);
				if(!(res.childs instanceof Array))
				{
					res.childs = [res.childs];
				}
			}
		}
		return res;
	},
};

///////////////////////////////////////////////
e.render = function render(mode)
{
	if('childs' in this)
	{
		let t = arguments.callee.t;
		if(!t)
		{
			t = router.createTemplate();
			t.compile(<{e.tag}>{t.childs}</{e.tag}>);
			arguments.callee.t = t;
		}
		t = t.clone();
		t.childs = this.childs.map(function(v) v.render(mode));
		return t;
	}

	let te = arguments.callee.te;
	if(!te)
	{
		te = router.createTemplate();
		te.compile(<{e.tag}/>);
		arguments.callee.te = te;
	}
	return te;
}


e.__proto__ = exec('super.js');
return e;