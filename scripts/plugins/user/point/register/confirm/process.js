
if(request.params.secret)
{
	let user = orm.query('SELECT id FROM {User} WHERE status=$1', 'waitConfirm:'+request.params.secret)[0];
	if(user)
	{
		orm.exec("UPDATE {User} SET status='ok' WHERE id=$1", user.id);

		router.cd.session.userId = user.id;
		global.user = user;

		router.cd.global.session.remember = false;
		request.pushHeader('Set-Cookie', 'sid='+router.cd.global.session.id+'; path=/');

		ui.blocks.center.push(this.properties.renderOk());


	}
	else
	{
		ui.blocks.center.push(this.render());
	}
}

ui.print();
