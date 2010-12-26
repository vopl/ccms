ui.reset();
ui.agent = {};
ui.agent.capabilites = {};


//ui.agent.type
if(/application\/xhtml\+xml/.test(request.env.accept||''))
{
	ui.agent.type = 'xhtml';
}
else if(/MSIE/.test(request.env['user-agent']||''))
{
	ui.agent.type = 'html';
}
else if(/\*\/\*/.test(request.env.accept||''))
{
	ui.agent.type = 'xhtml';
}
else
{
	ui.agent.type = 'html';
}








//ui.agent.capabilites.cookies = undefined;
//ui.agent.capabilites.cookies = false;
//ui.agent.capabilites.cookies = true;
ui.agent.capabilites.cookies = undefined;
//for(let stub in request.cookies) {ui.agent.capabilites.cookies = true; break;}
//if(!ui.agent.capabilites.cookies)
//{
//	request.pushHeader('Set-Cookie', 'uict='+crypto.rand.str_(4)+'; path=/');
//}








//ui.agent.capabilites.javaScript = undefined;
//ui.agent.capabilites.javaScript = false;
//ui.agent.capabilites.javaScript = true;
ui.agent.capabilites.javaScript = undefined;
