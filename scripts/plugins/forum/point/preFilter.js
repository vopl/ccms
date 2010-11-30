ui.blocks.breadcrumbs.front = this.parent.properties.breadcrumbs();


{
	let t = arguments.callee.header;
	if(!t)
	{
		t = router.createTemplate();
		t.compile(<>
			<table border="1">
				<tr>
					<td>
						<img src='/dog.png.s'/>
					</td>
					<td width='100%' align="center">
						тут
					</td>
					<td align='right'>
						{t.auth}
					</td>
				</tr>
			</table>
		</>);
		t.auth = router.getPoint('/user/authorize');
		arguments.callee.header = t;
	}
	t = t.clone();
	//t.auth = ui.skin.link(t.auth.render());
	t.auth = ui.skin.sidebar(t.auth.render(), "Auth");
	ui.blocks.header.push(t);

	ui.blocks.right.push(ui.skin.sidebar('content1', 'header1'));
	ui.blocks.right.push(ui.skin.sidebar('content2', 'header2'));
	ui.blocks.right.push(ui.skin.sidebar('content3', 'header3'));
}
