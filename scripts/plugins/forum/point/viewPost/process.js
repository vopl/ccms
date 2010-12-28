if(request.params.mode && request.params.mode=='update')
{
	let res = this.properties.renderRow(request.planData.post);

	request.pushHeader('Content-Type', "text/xml; charset=utf-8");
	print('<?xml version="1.0" encoding="utf-8"?>\n');
	print('<update revision="', Date.new().toUTCString(), '">')
		print('<set>')
			res.print();
		print('</set>');
		print('<reset>');
		print('</reset>');
	print('</update>');
	return;
}

ui.blocks.center.push(
	acl.hasRight('forum', 'viewPost')?
		this.render():
		ui.skin.accessDenied());
ui.print();



