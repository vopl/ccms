let manager = arguments[0];

let te = 
{
	id:arguments[1],
	options:arguments[2],
};

te.elements = 
{
	text:exec('elements/text.js'),

	bold:exec('elements/bold.js'),
	italic:exec('elements/italic.js'),
	underline:exec('elements/underline.js'),
	strike:exec('elements/strike.js'),
	subscript:exec('elements/subscript.js'),
	superscript:exec('elements/superscript.js'),
	deletion:exec('elements/deletion.js'),
	insertion:exec('elements/insertion.js'),


	// font:exec('elements/font.js'),
	// size:exec('elements/size.js'),
	// color:exec('elements/color.js'),
	// bgcolor:exec('elements/bgcolor.js'),


	// align:exec('elements/align.js'),
	// pagebreak:exec('elements/pagebreak.js'),
	// hr:exec('elements/hr.js'),
	// br:exec('elements/br.js'),
	// ident:exec('elements/ident.js'),
	// p:exec('elements/p.js'),



	// caption:exec('elements/caption.js'),
	// spoiler:exec('elements/spoiler.js'),
	// whisper:exec('elements/whisper.js'),
	// citate:exec('elements/citate.js'),
	// abbreviation:exec('elements/abbreviation.js'),
	// acronym:exec('elements/acronym.js'),
	// code:exec('elements/code.js'),


	// table:exec('elements/table.js'),
	// list:exec('elements/list.js'),
	// nlist:exec('elements/nlist.js'),


	// smile:exec('elements/smile.js'),
	// url:exec('elements/url.js'),
	// urlExt:exec('elements/urlExt.js'),
	// email:exec('elements/email.js'),
	// anchor:exec('elements/anchor.js'),


	// fileExt:exec('elements/fileExt.js'),
	// imageExt:exec('elements/imageExt.js'),
	// audioExt:exec('elements/audioExt.js'),
	// videoExt:exec('elements/videoExt.js'),
	// flashExt:exec('elements/flashExt.js'),


	// file:exec('elements/file.js'),
	// image:exec('elements/image.js'),
	// audio:exec('elements/audio.js'),
	// video:exec('elements/video.js'),
	// flash:exec('elements/flash.js'),


	// script:exec('elements/script.js'),

	// custom:exec('elements/custom.js'),
}

////////////////////////////////////////////////////////////
te.parserTables = 
{
	'element':[],
	'comment':[],
	'processing-instruction':[],
	'text':[],
};
te.parserTablesElement = {};

////////////////////////////////////////////////////////////
//заполнить таблицы парсеров
for (let ename in te.elements)
{
	let element = te.elements[ename];

	switch(element.parser.kind)
	{
	case 'element':
		for(let tag in element.parser.tags)
		{
			let priority = element.parser.tags[tag];
			if(!te.parserTablesElement[tag]) te.parserTablesElement[tag] = [];
			te.parserTablesElement[tag].push({parser:element.parser.parse, priority:priority});
		}
		break;
	case 'comment':
	case 'processing-instruction':
	case 'text':
		let table = te.parserTables[element.parser.kind];
		let priority = element.parser.priority?element.parser.priority:0;
		table.push({parser:element.parser.parse, priority:priority});
	}
}

for(let kind in te.parserTables)
{
	if('element' == kind)
	{
		for(let tag in te.parserTablesElement)
		{
			te.parserTablesElement[tag] = te.parserTablesElement[tag].sort(function(a,b) a.priority<b.priority);
			te.parserTablesElement[tag] = te.parserTablesElement[tag].map(function(a) a.parser);
		}
	}
	else
	{
		te.parserTables[kind] = te.parserTables[kind].sort(function(a,b) a.priority<b.priority);
		te.parserTables[kind] = te.parserTables[kind].map(function(a) a.parser);
	}
}

////////////////////////////////////////////////////////////
te.parserTables.element.push(function(xml, context, manager)
{
	let parsers = te.parserTablesElement[xml.name()];
	if(parsers)
	{
		for each(let parser in parsers)
		{
			let res = parser(xml, context, manager);
			if(res) return res;
		}
	}
	let parsers = te.parserTablesElement['*'];
	if(parsers)
	{
		for each(let parser in parsers)
		{
			let res = parser(xml, context, manager);
			if(res) return res;
		}
	}
	warn('unable to parse element: '+xml.toXMLString());
	return null;
});

te.parse = function parse(xml, context)
{
	if(!context) context = {};
	switch(xml.length())
	{
	case 0:
		return null;
	case 1:
		let parserTable = this.parserTables[xml.nodeKind()];
		if(!parserTable)
		{
			warn('unknown node kind for evaluate parser table: '+xml.nodeKind());
			return null;
		}

		for each(let parser in parserTable)
		{
			let res = parser(xml, context, this);
			if(res) return res;
		}
		warn('unable to parse: '+xml.toXMLString());
		return null;
	default:
		let res = [];
		for each(let c in xml)
		{
			let pr = this.parse(c, context);
			if(pr) res.push(pr);
		}
		return res;
	}
	//return DomTree
}

te.render_internal = function render_internal(domTree, context)
{
	let res = [];
	if(!domTree) return res;
	if(!(domTree instanceof Array)) domTree = [domTree];

	for each(let element in domTree)
	{
		res.push(element.render_internal(context));
	}

	return res;
}

te.render_web = function render_web(domTree, context)
{
	let res = [];
	if(!domTree) return res;
	if(!(domTree instanceof Array)) domTree = [domTree];

	for each(let element in domTree)
	{
		res.push(element.render_web(context));
	}

	return res;
}


if(!te.options.engine) te.options.engine = "tmce";
switch(te.options.engine)
{
case 'tmce':
	te.engine = exec('te_tmce.js', te);
	break;
default:
	throw Error("te.options.engine is not valid: "+te.options.engine);
}

te.render = function render(isid, doc)
{
	if(!isid) throw Error('isid must be provided');

	if('object' != typeof(doc)) throw Error('doc must be an object with attributes "dom" and "data"');
	if(!doc.data) doc.data = {};
	if(!doc.dom) doc.dom = null;
	else if('xml' == typeof(doc.dom))
	{
		doc.dom = this.parse(doc.dom, doc.data);
	}

	return this.engine.render(isid, doc);
}

return te;
