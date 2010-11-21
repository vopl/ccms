var session = router.cd.global.session;
delete router.cd.global.session;

if(session.needUpdate)
{
	session.data = JSON.stringify(router.cdGetInstance('session'));

	if(session.isNew)
	{
		orm.exec('INSERT INTO {Session} (id, ctime, atime, dtime, data, addr, remember) VALUES ($1,$2,$3,$4,$5,$6,$7)', 
			session.id, session.ctime, session.atime, session.dtime, session.data, session.addr, session.remember);
		delete session.isNew;
	}
	else
	{
		orm.exec('UPDATE {Session} SET atime=$1, dtime=$2, data=$3, addr=$4, remember=$5 WHERE id=$6', 
			session.atime, session.dtime, session.data, session.addr, session.remember, session.id);
	}
	session.data = router.cdGetInstance('session');
	session.needUpdate = false;
}

