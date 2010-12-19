let te = arguments[0];
let tmce = {};


//шаблон для формы редактора и скрипт инициализации
let t = router.createTemplate();
t.compile(<>
	<textarea id={t.isid+"_content"} name="content" rows="15" cols="80" style="width: 80%">{t.content}</textarea><br/>
	<script type="text/javascript">{t.scriptBody}</script>
</>);

//текстовый шаблон для предотвращения xml-вставки в textarea у формы
let tcontent = router.createTemplate();
tcontent.compileText(tcontent.content);




///////////////////////////////////////
// преобразование из внутреннего представления в веб
tmce.i2e = function i2e(doc)
{
	let res = tcontent.clone();
	res.content = te.renderDoc(doc, 'web');;
	return res;
}



///////////////////////////////////////
// преобразование из веб представления во внутреннее
tmce.e2i = function e2i(text)
{
	text = '<root>'+xmlEnties2Codes(text)+'</root>';
	let xml = new XML(text);
	xml = xml.children();

	//тут обход дерева и всякие замены представления
	for each(let c in xml) this.e2i_walker(c);

	let doc = te.parse(xml);
	return doc;
}

//////////////////////////////////////
// преобразование одного узла
tmce.e2i_walker = function e2i_walker(node)
{
	switch(node.nodeKind())
	{
	case 'element':
		this.e2i_walkerElement(node);
		break;
	case 'attribute':
	case 'comment':
	case 'processing-instruction':
	case 'text':
		break;
	}
}

//////////////////////////////////////
// преобразование одного тэга
tmce.e2i_walkerElement = function e2i_walkerElement(e)
{
	switch(String(e.name()))
	{
	case 'em':		e.setName('i');		break;
	case 'strong':		e.setName('b');		break;
	case 'strike':		e.setName('s');		break;
	case 'underline':	e.setName('u');		break;

	case 'span':
		let style = String(e.@style);
		let styles=[];
		let spair = /([^\s\:]+)\s*\:\s*([^\;]+)/.exec(style);
		if(spair)
		{
			let key = spair[1];
			let value = spair[2];

			switch(key)
			{
			case 'text-decoration':
				switch(value)
				{
				case 'line-through':	e.setName('s');		break;
				case 'underline':	e.setName('u');		break;
				}
				break;
			case 'font-style':
				switch(value)
				{
				case 'italic':		e.setName('i');		break;
				}
				break;
			case 'font-weight':
				switch(value)
				{
				case 'bold':		e.setName('b');		break;
				}
				break;
			}
		}
		break;
	}

	for each(let c in e) this.e2i_walker(c);
}


///////////////////////////////////////
// формирование xhtml редактора с идентификатором экземпляра isid и начальным состоянием data
tmce.render = function render(isid, doc)
{
	let lt = t.clone();
	lt.isid = isid;

	if(doc)
	{
		lt.content = this.i2e(doc);
	}
	else
	{
		lt.content = '';
	}


	/////////////////////////////////
	//сформировать перечень кнопок визивига и перечень разрешенных тегов
	let valid_elements = {};	//http://wiki.moxiecode.com/index.php/TinyMCE:Configuration/valid_elements
	let buttons = {};		//http://wiki.moxiecode.com/index.php/TinyMCE:Control_reference
	let plugins = {};

	for each(let e in te.elements)
	{
		switch(e.kind)
		{
		case 'bold':
			valid_elements['b/strong'] = true;
			buttons.bold=true;
			break;
		case 'underline':
			valid_elements.u = true;
			//valid_elements['span[style]'] = true;
			buttons.underline = true;
			break;
		case 'strike':
			valid_elements.s = true;
			//valid_elements['span[style]'] = true;
			buttons.strikethrough = true;
			break;
		case 'insertion':
			valid_elements.ins = true;
			buttons.ins = true;
			plugins.xhtmlxtras = true;
			break;
		case 'deletion':
			valid_elements.del = true;
			buttons.del = true;
			plugins.xhtmlxtras = true;
			break;
		case 'italic':
			valid_elements.em = true;
			buttons.italic = true;
			break;
		case 'subscript':
			valid_elements.sub = true;
			buttons.sub = true;
			break;
		case 'superscript':
			valid_elements.sup = true;
			buttons.sup = true;
			break;
		case 'paragraph':
			valid_elements.p = true;
			break;
		case 'header':
			valid_elements.h1 = true;
			valid_elements.h2 = true;
			valid_elements.h3 = true;
			valid_elements.h4 = true;
			valid_elements.h5 = true;
			valid_elements.h6 = true;
			break;
		}
	}

	for each(let f in te.facilities)
	{
		switch(f)
		{
		case 'source':
			buttons.code=true;
			break;
		case 'preview':
			buttons.preview = true;
			plugins.preview = true;
			break;
		case 'save':
			buttons.save = true;
			plugins.save = true;
			break;
		case 'cancel':
			buttons.cancel = true;
			plugins.save = true;
			break;
		}
	}




	/////////////////////////////////
	valid_elements = [].pushKeys(valid_elements);
	buttons = [].pushKeys(buttons);
	plugins = [].pushKeys(plugins);
	/////////////////////////////////


	//http://wiki.moxiecode.com/index.php/TinyMCE:Configuration
	t.scriptBody = ["tinyMCE.init({\n"];
	t.scriptBody.push('mode : "exact",\n');
	t.scriptBody.push('elements : "'+isid+"_content"+'",\n');

	t.scriptBody.push('theme:"advanced",\n');
	t.scriptBody.push('doctype:"<!DOCTYPE html PUBLIC \'-//W3C//DTD XHTML 1.0 Strict//EN\' \'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\'>",\n');

	//t.scriptBody.push('valid_elements:"'+valid_elements.join(',')+'",\n');
	t.scriptBody.push('theme_advanced_buttons1 : "'+buttons.join(',')+'",\n');
	t.scriptBody.push('theme_advanced_buttons2 : "",\n');
	t.scriptBody.push('theme_advanced_buttons3 : "",\n');
	t.scriptBody.push('theme_advanced_buttons4 : "",\n');
	t.scriptBody.push('theme_advanced_buttons5 : "",\n');
	t.scriptBody.push('theme_advanced_buttons6 : "",\n');
	t.scriptBody.push('plugins : "'+plugins.join(',')+'",\n');




	t.scriptBody.push("});");

	ui.blocks.scripts.tmce = "/tinymce/jscripts/tiny_mce/tiny_mce_src.js";

	return lt;
}


return tmce;