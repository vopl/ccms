let tmce = {};



let t = router.createTemplate();
t.compile(<>
	isid: {t(function() dumps(t.obj.isid))}<br/>
	content: {t(function() dumps(t.obj.content))}<br/>
	images: {t(function() dumps(t.obj.images))}<br/>
	files: {t(function() dumps(t.obj.files))}<br/>

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

///////////////////////////////////////
// формирование xhtml редактора с идентификатором экземпляра isid и начальным состоянием data
tmce.render = function render(isid, dataInt)
{
	let lt = t.clone();
	isid = "asdf";
	lt.isid = isid;

	if(dataInt)
	{
		lt.content = this.renderContent(dataInt.content);
		lt.files = dataInt.files;
		lt.images = dataInt.images;
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
	t.scriptBody.push('doctype:"<!DOCTYPE html PUBLIC \'-//W3C//DTD XHTML 1.0 Strict//EN\' \'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\'>",\n');

	t.scriptBody.push('valid_elements:"a[href],span,div[class|id]",\n');
	t.scriptBody.push('content_css:"/null.css",\n');
	t.scriptBody.push('verify_css_classes:true,\n');
	t.scriptBody.push('entity_encoding:"named",\n');




	t.scriptBody.push("});");

	ui.blocks.scripts.tmce = "/tinymce/jscripts/tiny_mce/tiny_mce.js";

	return lt;
}

///////////////////////////////////////
// из редактора во внутренний формат
tmce.ext2int = function ext2int(dataExt)
{
	//return {...};
	return dumps(arguments);
}

///////////////////////////////////////
// из внутреннего формата в редактор
tmce.int2ext = function int2ext(dataInt)
{
	//return {...};
	return dumps(arguments);
}

///////////////////////////////////////
// из внутреннего формата в отображаемый xhtml
tmce.int2vis = function int2vis(dataInt)
{
	//return template;
	return dumps(arguments);
}

return tmce;