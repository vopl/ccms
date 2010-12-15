let manager = arguments[0];

let te = 
{
	id:arguments[1],
	options:arguments[2],
};

















////////////////////////////////////////////////////////////
te.elements = {};


////////////////////////////////////////////////////////////
//отработать te.options.elements.allow, te.options.elements.disallow
if(!('elements' in te.options)) te.options.elements = {};
if(!('allow' in te.options.elements))
{
	te.options.elements.allow = 
	{
		text:true,
		textTag:true,

		bold:true,
		italic:true,
		underline:true,
		strike:true,
		insertion:true,
		deletion:true,
		subscript:true,
		superscript:true,


		// font:true,
		// size:true,
		// color:true,
		// bgcolor:true,


		// align:true,
		// pagebreak:true,
		// hr:true,
		// br:true,
		// ident:true,
		// p:true,



		// caption:true,
		// spoiler:true,
		// whisper:true,
		// citate:true,
		// abbreviation:true,
		// acronym:true,
		// code:true,


		// table:true,
		// list:true,
		// nlist:true,


		// smile:true,
		// url:true,
		// urlExt:true,
		// email:true,
		// anchor:true,


		// fileExt:true,
		// imageExt:true,
		// audioExt:true,
		// videoExt:true,
		// flashExt:true,


		// file:true,
		// image:true,
		// audio:true,
		// video:true,
		// flash:true,


		// script:true,

		// custom:true,
	};
}
else
{
	if(te.options.elements.allow instanceof Array)
	{
		let allow = {};
		for each(let ename in te.options.elements.allow) allow[ename] = true;
		te.options.elements.allow = allow;
	}

	te.options.elements.allow.text=true;
	te.options.elements.allow.textTag=true;
}

if('disallow' in te.options.elements)
{
	if(te.options.elements.disallow instanceof Array)
	{
		for each(let ename in te.options.elements.disallow)
		{
			delete te.options.elements.allow[ename];
		}
	}
	else
	{
		for(let ename in te.options.elements.disallow)
		{
			delete te.options.elements.allow[ename];
		}
	}
}

////////////////////////////////////////////////////////////
//сформировать экземпляры элементов
for(let ename in te.options.elements.allow)
{
	try
	{
		te.elements[ename] = exec('./elements/'+ename+'.js');
	}
	catch(e)
	{
		warn('exception when load element driver "'+ename+'" for text editor: '+e);
	}
}














////////////////////////////////////////////////////////////
te.facilities = {};


////////////////////////////////////////////////////////////
//отработать te.options.facilities.allow, te.options.facilities.disallow
if(!('facilities' in te.options)) te.options.facilities = {};
if(!('allow' in te.options.facilities))
{
	te.options.facilities.allow = 
	{
		source:true,
		preview:true,
	};
}
else
{
	if(te.options.facilities.allow instanceof Array)
	{
		let allow = {};
		for each(let ename in te.options.facilities.allow) allow[ename] = true;
		te.options.facilities.allow = allow;
	}
}

if('disallow' in te.options.facilities)
{
	if(te.options.facilities.disallow instanceof Array)
	{
		for each(let ename in te.options.facilities.disallow)
		{
			delete te.options.facilities.allow[ename];
		}
	}
	else
	{
		for(let ename in te.options.facilities.disallow)
		{
			delete te.options.facilities.allow[ename];
		}
	}
}

////////////////////////////////////////////////////////////
//сформировать экземпляры элементов
for(let fname in te.options.facilities.allow)
{
	te.facilities[fname] = fname;
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
		break;
	default:
		throw Error('unknown element kind: '+element.parser.kind);
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
te.parserTables.element.push(function(xml, manager)
{
	let parsers = te.parserTablesElement[xml.name()];
	if(parsers)
	{
		for each(let parser in parsers)
		{
			let res = parser(xml, manager);
			if(res) return res;
		}
	}
	let parsers = te.parserTablesElement['*'];
	if(parsers)
	{
		for each(let parser in parsers)
		{
			let res = parser(xml, manager);
			if(res) return res;
		}
	}
	warn('unable to parse element: '+xml.toXMLString());
	return null;
});

te.parse = function parse(xml)
{
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
			let res = parser(xml, this);
			if(res) return res;
		}
		warn('unable to parse: '+xml.toXMLString());
		return null;
	default:
		let res = [];
		for each(let c in xml)
		{
			let pr = this.parse(c);
			if(pr) res.push(pr);
		}
		return res;
	}
	//return DomTree
}


/////////////////////////////////////////////////////////
te.render_internal = function render_internal(doc)
{
	let res = [];
	if(!doc) return res;
	if(!(doc instanceof Array)) doc = [doc];

	for each(let element in doc)
	{
		res.push(element.render_internal());
	}

	return res;
}


/////////////////////////////////////////////////////////
te.render_web = function render_web(doc)
{
	let res = [];
	if(!doc) return res;
	if(!(doc instanceof Array)) doc = [doc];

	for each(let element in doc)
	{
		res.push(element.render_web());
	}

	return res;
}


/////////////////////////////////////////////////////////
te.render = function render(isid, doc)
{
	if(!isid) throw Error('isid must be provided');

	if(!doc) doc = null;
	else if('xml' == typeof(doc))
	{
		doc = this.parse(doc);
	}

	return this.engine.render(isid, doc);
}

/////////////////////////////////////////////////////////
if(!te.options.engine) te.options.engine = "tmce";
switch(te.options.engine)
{
case 'tmce':
	te.engine = exec('te_tmce.js', te);
	break;
default:
	throw Error("te.options.engine is not valid: "+te.options.engine);
}
return te;
