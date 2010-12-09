let manager = arguments[0];

let te = 
{
	id:arguments[1],
	options:arguments[2],
};

te.elements = 
{
	bold:true,
	italic:true,
	underline:true,
	strike:true,
	subscript:true,
	superscript:true,
	deletion:true,
	insertion:true,


	font:true,
	size:true,
	color:true,
	bgcolor:true,


	align:true,
	layer:true,
	pagebreak:true,
	hr:true,
	br:true,
	ident:true,



	caption:true,
	spoiler:true,
	whisper:true,
	citate:true,
	abbreviation:true,
	acronym:true,
	code:true,


	table:true,
	list:true,
	nlist:true,


	smile:true,
	url:true,
	urlExt:true,
	email:true,
	anchor:true,


	fileExt:true,
	imageExt:true,
	audioExt:true,
	videoExt:true,
	flashExt:true,


	file:true,
	image:true,
	audio:true,
	video:true,
	flash:true,


	script:true,

	custom:true,
}

te.parseContent = function parse(xhtml)
{
	//return DomTree
}

te.renderContent = function parse(domTree)
{
	//return xhtml
}


if(!te.options.engine) te.options.engine = "tmce";
switch(te.options.engine)
{
case 'tmce':
	te.engine = exec('te_tmce.js');
	break;
default:
	throw Error("te.options.engine is not valid: "+te.options.engine);
}

te.render = function render(isid, data)
{
	if(!isid) isid = manager.genIsid(this.id);
	return this.engine.render(isid, data);
}

return te;
