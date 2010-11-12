if(global.user.login!='admin') {print(ui.skin.accessDenied()); return}

let doRedirect = false;

if(request.params.saveToBase)
{
	system.saveToDb_options();
	doRedirect = true;
}
else
{
	if(request.params.name && String(request.params.name) in system.options)
	{
		system.options[request.params.name].value = request.params.value;
		doRedirect = true;
	}
}

if(doRedirect)
{
	request.setStatusCode(302);
	request.pushHeader('Location', this.path);
}
else
{
	this.render();
	//print('<pre>');
	//dump(system.options);
	//print('</pre>');
}
