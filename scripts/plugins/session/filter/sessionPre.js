
function callback(v)
{
//	warn('session droped from cache callback: '+dumps(v));
}


let session;
if(request.cookies.sid)
{
	session = global.cache.process({
		key:'session.'+request.cookies.sid,
		provider:function()
		{
			let res = orm.query('SELECT * FROM {Session} WHERE id=$1', request.cookies.sid)[0];
			if(res)
			{
				res.data = JSON.parse(res.data);
			}
			return res;
		},
		callback:callback,
	});
}

let now = new Date();
if(!session)
{
	session = {
		id:String(Math.random()),
		ctime:now,
		atime:now,
		dtime:now,
		data:{},
		addr:request.env.REMOTE_ADDR,
	};
	session.isNew = true,
	session.needUpdate = true;
	request.pushHeader('Set-Cookie', 'sid='+session.id+'; path=/');
	
	global.cache.set('session.'+session.id, session, null, null, callback);
}
else
{
	if((now.getTime() - session.atime.getTime())/1000 > 10)
	{
		session.atime = now;
		session.needUpdate = true;
	}
}

session.dtime = new Date(session.atime.getTime() + 2*60*60*1000);
//session.dtime = new Date(session.atime.getTime() + 10*1000);


router.cdSetInstance('session', session.data);
router.cd.global.session = session;
