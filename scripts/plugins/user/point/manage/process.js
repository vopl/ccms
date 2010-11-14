
ui.blocks.center.push(
	acl.hasRight('user', 'manage')?
		this.render():
		ui.skin.accessDenied());
ui.print();


