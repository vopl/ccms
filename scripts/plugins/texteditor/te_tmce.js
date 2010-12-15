let te = arguments[0];
let tmce = {};


//шаблон для формы редактора и скрипт инициализации
let t = router.createTemplate();
t.compile(<>
	<form method="post" action="/texteditor">
		<input type="hidden" name="isid" value={t.isid}/>
		<input type="hidden" name="mode" value='render'/>
		<textarea id={t.isid+"_content"} name="content" rows="15" cols="80" style="width: 80%">
			{t.content}
		</textarea><br/>
		<input type="submit" name="save" value="Submit" />
		<input type="reset" name="reset" value="Reset" />
	</form>


	<script type="text/javascript">{t.scriptBody}</script>
</>);

//текстовый шаблон для предотвращения xml-вставки в textarea у формы
let tcontent = router.createTemplate();
tcontent.compileText(tcontent.content);

///////////////////////////////////////
// формирование xhtml редактора с идентификатором экземпляра isid и начальным состоянием data
tmce.render = function render(isid, doc)
{
	let lt = t.clone();
	lt.isid = isid;

	if(doc)
	{
		lt.content = tcontent.clone(); lt.content.content = te.render_web(doc);
		lt.files = 'doc.data.files';
		lt.images = 'doc.data.images';
	}
	else
	{
		lt.content = '';
		lt.files = '';
		lt.images = '';
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
			valid_elements.strong = true;
			buttons.bold=true;
			break;
		case 'underline':
			valid_elements.u = true;
			valid_elements['span[style]'] = true;
			buttons.underline = true;
			break;
		case 'strike':
			valid_elements.s = true;
			valid_elements['span[style]'] = true;
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

	t.scriptBody.push('valid_elements:"'+valid_elements.join(',')+'",\n');
	t.scriptBody.push('theme_advanced_buttons1 : "'+buttons.join(',')+'",\n');
	t.scriptBody.push('theme_advanced_buttons2 : "",\n');
	t.scriptBody.push('theme_advanced_buttons3 : "",\n');
	t.scriptBody.push('theme_advanced_buttons4 : "",\n');
	t.scriptBody.push('theme_advanced_buttons5 : "",\n');
	t.scriptBody.push('theme_advanced_buttons6 : "",\n');
	t.scriptBody.push('plugins : "'+plugins.join(',')+'",\n');




	t.scriptBody.push("});");

	ui.blocks.scripts.tmce = "/tinymce/jscripts/tiny_mce/tiny_mce.js";

	return lt;
}


return tmce;