
function callback(v)
{
	v.data = JSON.stringify(v.data);

	if(session.isNew)
	{
		//warn('insert session '+v.id);
		orm.exec('INSERT INTO {Session} (id, ctime, atime, dtime, data, addr, remember) VALUES ($1,$2,$3,$4,$5,$6,$7)', 
			v.id, v.ctime, v.atime, v.dtime, v.data, v.addr, v.remember);
	}
	else
	{
		try
		{
			//warn('update session '+v.id);
			orm.exec('UPDATE {Session} SET atime=$1, dtime=$2, data=$3, addr=$4, remember=$5 WHERE id=$6', 
				v.atime, v.dtime, v.data, v.addr, v.remember, v.id);
		}
		catch(e)
		{
			//warn('f update session '+v.id);
			// warn('session update fail!!!. Was deleted from cron?')
			// dumpe(e);
			orm.exec('INSERT INTO {Session} (id, ctime, atime, dtime, data, addr, remember) VALUES ($1,$2,$3,$4,$5,$6,$7)', 
				v.id, v.ctime, v.atime, v.dtime, v.data, v.addr, v.remember);
		}
	}
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
				//warn('load session '+res.id);
			}
			else
			{
				//warn('no load session '+request.cookies.sid);
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
		//id:hash.md5_(String(Math.random()), global.uniq, now.getTime()),
		id:rand.str_(22),
		ctime:now,
		atime:now,
		dtime:now,
		data:{},
		addr:request.env.REMOTE_ADDR,
		isNew:true,
	};
	request.pushHeader('Set-Cookie', 'sid='+session.id+'; path=/');
	global.cache.set('session.'+session.id, session, null, null, callback);
	
	//warn('create session '+session.id);
}
else
{
	session.atime = now;
}

session.dtime = new Date(session.atime.getTime() + 2*60*60*1000);


router.cdSetInstance('session', session.data);
router.cd.global.session = session;
