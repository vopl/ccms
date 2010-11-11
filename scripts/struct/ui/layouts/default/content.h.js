let t = arguments.callee.t;


if(!t)
{
	t = router.createTemplate();
	let xml = <>
	<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-us" lang="en-us" >
		<head>
			{t(ui.hookPlace(this.properties.name, 'preTagHead'))}

			{t(ui.places.title.render())}
			{t(ui.places.scripts.render())}

			{t(ui.hookPlace(this.properties.name, 'postTagHead'))}
		</head>
		<body>
			{t(ui.hookPlace(this.properties.name, 'preTagBody'))}
			<table border='1'>

			<tr>
				<td colspan='3'>
					{t(ui.places.header.render())}
				</td>
			</tr>
			<tr>
				<td>
					{t(ui.places.left.render())}
				</td>
				<td width='100%'>
					{t(ui.places.center.render())}
				</td>
				<td>
					{t(ui.places.right.render())}
				</td>
			</tr>
			<tr>
				<td colspan='3'>
					{t(ui.places.footer.render())}
				</td>
			</tr>

			</table>

			{t(ui.hookPlace(this.properties.name, 'postTagBody'))}
		</body>
	</html></>;
	t.compile(xml);
	
	arguments.callee.t=t;
}
return t.clone();
