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
e.render = function render(mode)
{
	let t = e.template.clone();
	t.content = this.content;
	return t;
}

e.__proto__ = exec('super.js');
return e;