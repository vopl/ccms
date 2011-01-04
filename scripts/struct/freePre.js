if(request.cookies && request.cookies.PHPSESSID && request.cookies.marker)
//if(request.cookies.sid && request.cookies.vc)
{
	print('WTF? robot?');
	return 'complete';
}
return 'partial';
