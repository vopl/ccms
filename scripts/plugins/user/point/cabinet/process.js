if(!acl.hasRight('user', 'cabinet'))
{
	ui.blocks.center = ui.skin.accessDenied(); 
	ui.print();
	return;
}

ui.blocks.center = this.render;
ui.print();
