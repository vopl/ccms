let t = arguments.callee.t;


if(!t)
{
	t = router.createTemplate();
	let xml = <>
		<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-us" lang="en-us" >
			<head>
				{t(function(){return ui.places.title.render()})}
				<link rel='stylesheet' href={this.path+'/main.css'} type='text/css' media='all'/>
				{t(function(){return ui.places.styles.render()})}
				{t(function(){return ui.places.scripts.render()})}


			</head>
			<body>
				<div class="layout">

					<div class="layout-element layout-header">
						{t(function(){return ui.places.header.render()})}
					</div>
					
					<div class="layout-element layout-breadcrumbs">
						{t(function(){return ui.places.breadcrumbs.render()})}
					</div>
					
					<div class="layout-element layout-centerall">
						<div class="layout-element layout-left">
							{t(function(){return ui.places.left.render()})}
						</div>
						<div class="layout-element layout-right">
							{t(function(){return ui.places.right.render()})}
						</div>
						<div class="layout-element layout-center">
							{t(function(){return ui.places.center.render()})}
						</div>
					</div>
					<div class="layout-element layout-footer">
						{t(function(){return ui.places.footer.render()})}
					</div>

				</div>
				{t.ga}
				<b>{this.path}</b>
			</body>
			{t(function(){return ui.places.scriptsTail.render()})}
		</html>
	</>;
	t.compile(xml);
	
	arguments.callee.t=t;
}














// let t = arguments.callee.t;


// if(!t)
// {
	// t = router.createTemplate();
	// let xml = <>
	// <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-us" lang="en-us" >
		// <head>
			// {t(function(){return ui.places.title.render()})}
			// <?ws?>
			// {t(function(){return ui.places.scripts.render()})}
			// <link rel='stylesheet' href={this.path+'/main.css'} type='text/css' media='all'/>
			// {t(function(){return ui.places.styles.render()})}


		// </head>
		// <body>
			// <table border='1'>

			// <tr>
				// <td colspan='3'>
					// {t(function(){return ui.places.header.render()})}
				// </td>
			// </tr>
			// <tr>
				// <td colspan='3'>
					// {t(function(){return ui.places.breadcrumbs.render()})}
				// </td>
			// </tr>
			// <tr>
				// <td>
					// {t(function(){return ui.places.left.render()})}
				// </td>
				// <td width='100%'>
					// {t(function(){return ui.places.center.render()})}
				// </td>
				// <td>
					// {t(function(){return ui.places.right.render()})}
				// </td>
			// </tr>
			// <tr>
				// <td colspan='3'>
					// {t(function(){return ui.places.footer.render()})}
				// </td>
			// </tr>

			// </table>
			// {t.ga}
			// <b>{this.path}</b>
		// </body>
	// </html></>;
	// t.compile(xml);
	
	// arguments.callee.t=t;
// }

t = t.clone();

if(/graydog\.1gb\.ru/.test(request.env.host||''))
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
	t.ga = <>no ga for {request.env.host||''}<br/></>;
}

return t;
