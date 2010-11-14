ui.blocks.center.push(
	acl.hasRight('user', 'manage')?
		this.render():
		ui.skin.accessDenied());

ui.blocks.center.push(ui.mkBlockEntry('content1', 1));
ui.blocks.center.push(ui.mkBlockEntry('content4', 4));
ui.blocks.center.push(ui.mkBlockEntry('content6', 6));
ui.pushBlock('center', 'content-1', -1);

ui.pushBlock('center', 'content3', 3);
ui.pushBlock('center', 'content-2', -2);

ui.print();


