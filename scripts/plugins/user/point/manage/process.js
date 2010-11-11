ui.blocks.center = acl.hasRight('user', 'manage')?
	this.render():
	ui.skin.accessDenied();
ui.print();


