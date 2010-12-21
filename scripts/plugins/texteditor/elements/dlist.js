let e = {};

///////////////////////////////////////////////
e.kind = 'dlist';

///////////////////////////////////////////////
e.parser = 
{
	kind:'element',
	tags:{dl:0},
	parse: function parse(xml, manager)
	{
		let res = {childs:[]};
		res.__proto__ = e;

		for each(let li in xml)
		{
			if(li.nodeKind() != 'element') continue;

			let cel = {};
			switch(String(li.name()))
			{
			case 'dt':cel.type='dt'; break;
			case 'dd':cel.type='dd'; break;
			default: return null;
			}
			cel.content = manager.parse(li.children());
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
			t.compile(<dl>{t.childs}</dl>);
			arguments.callee.t = t;
		}
		let tdt = arguments.callee.tdt;
		if(!tdt)
		{
			tdt = router.createTemplate();
			tdt.compile(<dt>{tdt.content}</dt>);
			arguments.callee.tdt = tdt;
		}
		let tdd = arguments.callee.tli;
		if(!tdd)
		{
			tdd = router.createTemplate();
			tdd.compile(<dd>{tdd.content}</dd>);
			arguments.callee.tdd = tdd;
		}

		t = t.clone();
		t.childs = this.childs.map(function(v) {
			switch(v.type)
			{
			case 'dt':
				let tdtc = tdt.clone();
				tdtc.content = v.content.render(mode);
				return tdtc;
			case 'dd':
				let tddc = tdd.clone();
				tddc.content = v.content.render(mode);
				return tddc;
			};
			return undefined;
		});
		return t;
	}

	let te = arguments.callee.te;
	if(!te)
	{
		te = router.createTemplate();
		te.compile(<dl/>);
		arguments.callee.te = te;
	}
	return te;
}


e.__proto__ = exec('super.js');
return e;