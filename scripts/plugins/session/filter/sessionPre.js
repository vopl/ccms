
let session;
if(request.cookies.sid)
{
	session = orm.query('SELECT * FROM {Session} WHERE id=$1', request.cookies.sid)[0];
}

if(!session)
{
	session = {
		id:String(Math.random()),
		ctime:new Date(),
		//atime:new Date(),
		dtime:new Date(),
		data:{},
		addr:request.env.REMOTE_ADDR,
		isNew:true,
	};
	request.pushHeader('Set-Cookie', 'sid='+session.id+'; path=/');
}
else
{
	session.data = JSON.parse(session.data);
}


session.atime = new Date();
//session.dtime = new Date(session.atime.getTime() + 2*60*60*1000);
session.dtime = new Date(session.atime.getTime() + 10*1000);


router.cdSetInstance('session', session.data);
router.cd.global.session = session;
