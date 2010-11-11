let t;

if(global.user.login=='anonymous')
{
	t = this.properties.renderEmpty(arguments[0]);
}
else
{
	t = this.properties.renderUser(arguments[0]);
}


// return 	ui.sidebar({
		// title:_('Authorize'),
		// tooltip:_('Authorize tooltip text'),
		// content:t,
	// });

//let t1 = router.createTemplate();	
//t1.compile(<div>{t1.stub}</div>);
//t1.stub = t;
return t;
