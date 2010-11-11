
if(request.params.secret)
{
	let user = orm.query('SELECT id FROM {User} WHERE status=$1', 'waitConfirm:'+request.params.secret)[0];
	if(user)
	{
		orm.exec("UPDATE {User} SET status='ok' WHERE id=$1", user.id);
		ui.blocks.center = this.properties.renderOk();
	}
	else
	{
		ui.blocks.center = this.render();
	}
}

ui.print();
