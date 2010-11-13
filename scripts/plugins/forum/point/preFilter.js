if(!ui.blocks.breadcrumbs)
{
	ui.blocks.breadcrumbs = this.parent.properties.breadcrumbs();
}

if(!ui.blocks.header)
{
	let t = arguments.callee.header;
	if(!t)
	{
		t = router.createTemplate();
		
		t.compile(<>
			<table>
				<tr>
					<td>
						<img src='/dog.png'/>
					</td>
					<td width='100%'>
					</td>
					<td align='right'>
						{t.auth}
					</td>
				</tr>
			</table>
		</>);
		arguments.callee.header = t;
	}
	t = t.clone();
	t.auth = router.getPoint('/user/authorize').render();
	ui.blocks.header = t;
}
