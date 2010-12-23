let e = {};

///////////////////////////////////////////////
e.kind = 'pre';
e.settings = 'brush: plain';

///////////////////////////////////////////////
e.parser = 
{
	kind:'element',
	tags:{pre:0},
	parse: function parse(xml, manager)
	{
		let res = {};
		res.__proto__ = e;
		res.settings = xml.@class;
		res.childs = xml.children().toXMLString().replace(/&amp;/g, '&');
		res.childs = res.childs.replace(/<br\/>/g, '\n');
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
			t.compile(<pre class={t.settings}>{t.childs}</pre>);
			arguments.callee.t = t;
		}
		t = t.clone();
		t.settings = this.settings;
		t.childs = this.childs;
		return t;
	}

	let te = arguments.callee.te;
	if(!te)
	{
		te = router.createTemplate();
		te.compile(<pre class={te.settings}/>);
		arguments.callee.te = te;
	}
	te = te.clone();
	te.settings = this.settings;
	return te;
}


e.__proto__ = exec('super.js');
return e;