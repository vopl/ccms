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
	kind:'text',
	parse: function parse(xml, manager)
	{
		let res = {};
		res.__proto__ = e;
		res.content = xml.toXMLString();
		return res;
	},
};

///////////////////////////////////////////////
e.render = function render(mode)
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

e.__proto__ = exec('super.js');
return e;