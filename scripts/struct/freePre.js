if(request.cookie.PHPSESSID && request.cookie.marker)
//if(request.cookies.sid && request.cookies.vc)
{
	print('WTF? robot?');
	return 'complete';
}
return 'partial';
