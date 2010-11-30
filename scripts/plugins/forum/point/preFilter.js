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
	t.auth = t.auth.render();
	ui.blocks.header.push(t);
	//ui.pushBlock('header', t);
}
