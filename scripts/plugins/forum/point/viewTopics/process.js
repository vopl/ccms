ui.blocks.center.push(
	acl.hasRight('forum', 'viewPost')?
		this.render():
		ui.skin.accessDenied());
ui.print();



