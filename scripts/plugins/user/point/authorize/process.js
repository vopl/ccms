
try
{
	if(request.params.do)
	{
		switch(request.params.do)
		{
		case 'login':
			if(request.params.login)
			{
				let user = orm.User.select("WHERE login=$1", request.params.login)[0];
				if(!user)
				{
					this.msg = _("Login incorrect");
					break;
				}
				if(user.password != request.params.password)
				{
					this.msg = _("Password incorrect");
					break;
				}
				switch(user.status)
				{
				case 'ok':
					router.cd.session.userId = user.id;
					global.user = user;

					if(!acl.hasRight('user', 'login'))
					{
						this.msg=_('forbidden');
					}
					
					if(request.params.remember)
					{
						router.cd.global.session.remember = true;
						request.pushHeader('Set-Cookie', 'sid='+router.cd.global.session.id+"; expires="+(new Date((new Date()).getTime() + 7*30*24*60*60*1000))+'; path=/');
					}
					else
					{
						router.cd.global.session.remember = false;
						request.pushHeader('Set-Cookie', 'sid='+router.cd.global.session.id+'; path=/');
					}
					
					router.cd.global.session.needUpdate = true;
					request.setStatusCode(303);
					request.pushHeader('Location', request.params.backUrl?request.params.backUrl:'/');
					return;
				case 'ban':
					this.msg = _("Unable to login");
					break;
				default:
					if(/waitConfirm:/.test(user.status))
					{
						ui.blocks.center.push(this.properties.waitConfirm());
						ui.print();
						return;
					}
					this.msg = _("Unknown status: $value", {$value:user.status});
					break;
				}
				break;
			}
			this.msg = _("Login must be not empty");
			break;
		case 'logout':
			delete global.user;
			delete router.cd.session.userId;
			router.cd.global.session.remember = false;
			router.cd.global.session.needUpdate = true;
			request.pushHeader('Set-Cookie', 'sid='+router.cd.global.session.id+'; path=/');
			request.setStatusCode(303);
			request.pushHeader('Location', request.params.backUrl?request.params.backUrl:'/');
			return;
		}
	}
}
catch(e)
{
	warn(e);
}
ui.blocks.center.push(this.render());
ui.print();

