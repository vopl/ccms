let gs = router.cd.session;
let user;
if(gs.userId)
{
	user = global.cache.process({
		key:'user.'+gs.userId,
		provider:function()
		{
			return orm.User.load({id:gs.userId});
		},
		events:['user.user'],
	});
}
if(!user)
{
	user = global.cache.process({
		key:'user.anonymous',
		provider:function()
		{
			return orm.User.select('WHERE login=$1', 'anonymous')[0];
		},
		events:['user.user'],
	});
	gs.userId = user.id;
}

global.user = user;

