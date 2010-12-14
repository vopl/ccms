let te = arguments[0];
let tmce = {};


//шаблон для формы редактора и скрипт инициализации
let t = router.createTemplate();
t.compile(<>
	<form method="post" action="/texteditor">
		<input type="hidden" name="isid" value={t.isid}/>
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
		lt.content = tcontent.clone(); lt.content.content = te.render_web(doc.dom, doc.data);
		lt.files = 'doc.data.files';
		lt.images = 'doc.data.images';
	}
	else
	{
		lt.content = '';
		lt.files = '';
		lt.images = '';
	}

	//http://wiki.moxiecode.com/index.php/TinyMCE:Configuration
	t.scriptBody = ["tinyMCE.init({\n"];
	t.scriptBody.push('mode : "exact",\n');
	t.scriptBody.push('elements : "'+isid+"_content"+'",\n');
	//t.scriptBody.push('plugins : "autosave",\n');

	t.scriptBody.push('theme:"advanced",\n');
	//t.scriptBody.push('doctype:"<!DOCTYPE html PUBLIC \'-//W3C//DTD XHTML 1.0 Strict//EN\' \'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\'>",\n');

	//t.scriptBody.push('valid_elements:"b,i,a[href],span,div[class|id]",\n');
	//t.scriptBody.push('content_css:"/null.css",\n');
	//t.scriptBody.push('verify_css_classes:true,\n');
	//t.scriptBody.push('entity_encoding:"named",\n');




	t.scriptBody.push("});");

	ui.blocks.scripts.tmce = "/tinymce/jscripts/tiny_mce/tiny_mce.js";

	return lt;
}

return tmce;