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
	underline:exec('elements/underline.js'),,
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

te.parseContent = function parse(xml)
{
	//return DomTree
}

te.renderContent = function parse(domTree)
{
	//return xml
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
