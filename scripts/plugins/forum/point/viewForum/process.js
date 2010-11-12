ui.blocks.center = acl.hasRight('forum', 'view')?
	this.render():
	ui.skin.accessDenied();
ui.print();

