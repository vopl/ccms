if(!acl.hasRight('user', 'cabinet'))
{
	ui.blocks.center.push(ui.skin.accessDenied()); 
	ui.print();
	return;
}

ui.blocks.center.push(this.render);
ui.print();
