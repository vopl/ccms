let e = {};

///////////////////////////////////////////////
e.kind = 'olist';

///////////////////////////////////////////////
e.parser = 
{
	kind:'element',
	tags:{ol:0},
	parse: function parse(xml, manager)
	{
		let res = {childs:[]};
		res.__proto__ = e;

		for each(let li in xml)
		{
			if(li.nodeKind() != 'element') continue;
			if(String(li.name()) != 'li') return null;
			let cel = manager.parse(li.children());
			res.childs.push(cel);
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
			t.compile(<ol>{t.childs}</ol>);
			arguments.callee.t = t;
		}
		let tli = arguments.callee.tli;
		if(!tli)
		{
			tli = router.createTemplate();
			tli.compile(<li>{tli.content}</li>);
			arguments.callee.tli = tli;
		}

		t = t.clone();
		t.childs = this.childs.map(function(v) {
			let tlic = tli.clone();
			tlic.content = v.render(mode);
			return tlic;
		});
		return t;
	}

	let te = arguments.callee.te;
	if(!te)
	{
		te = router.createTemplate();
		te.compile(<ol/>);
		arguments.callee.te = te;
	}
	return te;
}


e.__proto__ = exec('super.js');
return e;