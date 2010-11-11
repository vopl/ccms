let t = arguments.callee.t;


if(!t)
{
	t = router.createTemplate();
	let xml = <>
	<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-us" lang="en-us" >
		<head>
			{t(function(){return ui.places.title.render()})}
			<?ws?>
			{t(function(){return ui.places.scripts.render()})}
		</head>
		<body>
			<table border='1'>

			<tr>
				<td colspan='3'>
					{t(function(){return ui.places.header.render()})}
				</td>
			</tr>
			<tr>
				<td colspan='3'>
					{t(function(){return ui.places.breadcrumbs.render()})}
				</td>
			</tr>
			<tr>
				<td>
					{t(function(){return ui.places.left.render()})}
				</td>
				<td width='100%'>
					{t(function(){return ui.places.center.render()})}
				</td>
				<td>
					{t(function(){return ui.places.right.render()})}
				</td>
			</tr>
			<tr>
				<td colspan='3'>
					{t(function(){return ui.places.footer.render()})}
				</td>
			</tr>

			</table>
			<b>{this.path}</b>
		</body>
	</html></>;
	t.compile(xml);
	
	arguments.callee.t=t;
}
return t.clone();
