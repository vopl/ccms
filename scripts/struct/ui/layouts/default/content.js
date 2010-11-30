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


			<style><![CDATA[
				.sidebar
				{
					margin-top: 5px;
					margin-bottom: 5px;
					vertical-align: top;
				}
				.sidebar-header
				{
					border: 1px double blue;
				}
				.sidebar-content
				{
					border: 1px double black;
				}
				]]>
			</style>
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
			{t.ga}
			<b>{this.path}</b>
		</body>
	</html></>;
	t.compile(xml);
	
	arguments.callee.t=t;
}

t = t.clone();

if(/graydog\.1gb\.ru/.test(request.env.host))
//if(/local/.test(request.env.host))
{
	t.ga = <>
		<script type="text/javascript"><![CDATA[

		  var _gaq = _gaq || [];
		  _gaq.push(['_setAccount', 'UA-19928951-1']);
		  _gaq.push(['_trackPageview']);

		  (function() {
			var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
			ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
			var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
		  })();

		]]></script><br/>
	</>;
}
else
{
	t.ga = <>no ga for {request.env.host}<br/></>;
}

return t;
