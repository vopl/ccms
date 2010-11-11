if(!ui.blocks.breadcrumbs)
{
	ui.blocks.breadcrumbs = this.parent.properties.breadcrumbs();
}

if(!ui.blocks.left)
{
	ui.blocks.left = router.getPoint('/user/authorize').render();
}
