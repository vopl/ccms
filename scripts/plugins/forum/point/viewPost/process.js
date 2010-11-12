ui.blocks.center = acl.hasRight('forum', 'viewPost')?
	this.render():
	ui.skin.accessDenied();
ui.print();



