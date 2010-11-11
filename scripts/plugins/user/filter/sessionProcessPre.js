let gs = router.cd.session;
let user;
if(gs.userId)
{
	user = orm.User.load({id:gs.userId});
}
if(!user)
{
	user = orm.User.select('WHERE login=$1', 'anonymous')[0];
	gs.userId = user.id;
}

global.user = user;

