ui.blocks.breadcrumbs.front = this.parent.properties.breadcrumbs();


{
	let t = arguments.callee.header;
	if(!t)
	{
		t = router.createTemplate();
		t.compile(<>
			<div class="header-left">
				{t.logo}
			</div>
			<div class="header-right">
				{t.auth}
			</div>
			<div class="header-center">
				тут
			</div>
		</>);
		t.auth = router.getPoint('/user/authorize');
		arguments.callee.header = t;
	}
	t = t.clone();

	t.logo = ui.skin.image('/dog.png');
	t.auth = ui.skin.sidebar(t.auth.render(), "Auth");
	ui.blocks.header.push(t);



	ui.blocks.right.push(ui.skin.sidebar('rcontent1', 'rheader1'));
	ui.blocks.right.push(ui.skin.sidebar('rcontent2', 'rheader2'));
	ui.blocks.right.push(ui.skin.sidebar('rcontent3', 'rheader3'));

	ui.blocks.left.push(ui.skin.sidebar('lcontent1', 'lheader1'));
	ui.blocks.left.push(ui.skin.sidebar('lcontent2', 'lheader2'));
	ui.blocks.left.push(ui.skin.sidebar('lcontent3', 'lheader3'));

	ui.blocks.footer.push(ui.skin.sidebar('fcontent3', 'fheader3'));


	ui.blocks.styles.forum = this.parent.path+'/main.css';

}




