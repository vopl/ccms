ui.blocks.center.push(
	acl.hasRight('forum', 'view')?
		this.render():
		ui.skin.accessDenied());
ui.print();

