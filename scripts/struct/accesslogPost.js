if(global.access)
{
	global.access.print('code ', request.getStatusCode(), "\r\n", request.getHeaders(), "\r\n", '}}} done', "\r\n");
}
