﻿this.msg = '';
try
{
	if(request.params.save)
	{
		if(!request.params.acceptAgreement)
		{
			throw 'надо принять соглашение';
		}
		
		if(!request.params.login)
		{
			throw 'логин нельзя пустой';
		}
		
		let count = orm.query('SELECT COUNT(*) FROM {User} WHERE login=$1', request.params.login)[0].count;
		if(count)
		{
			throw 'логин занят';
		}

		if(!request.params.password)
		{
			throw 'пароль не должен быть пустым';
		}
		if(request.params.password != request.params.password2)
		{
			throw 'пароль подтверждения не совпадает';
		}
		
		if(!request.params.email)
		{
			throw 'почтовый адрес надо указывать обязательно';
		}
		
		let emailRegex = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
		if(!emailRegex.test(request.params.email))
		{
			throw 'почтовый адрес не соответствует';
		}
		
		if(request.params.captchaCode != this.cd.session.captcha[request.params.captchaPassword])
		{
			throw 'капча не соответствует';
		}
		delete this.cd.session.captcha;
		
		let confirmSecret = Crypto.Rand.str(22);
		
		//влить пользователя в базу
		let roles = [].concat(
			orm.Role.select('WHERE module=$1 AND name=$2', 'user', 'user'),
			orm.Role.select('WHERE module=$1 AND name=$2', 'forum', 'user'));
		
		let user = orm.User.make({
				login:request.params.login,
				email:request.params.email,
				//status:'waitConfirm:'+confirmSecret,
				status:'ok',
				create_date:new Date(),
			});
		user.setPassword(request.params.password);
		user.save();
		
		acl.setUser(user, roles);
			
		//отправить письмо
		if(0)
		{
			let confirmHref = 'http://'+request.env.host+'/user/register/confirm?secret='+confirmSecret;
			let msg = new Mime.Part(
				{	from:router.getConfig().adminEmail,
					to:request.params.email,
					subject:'Подтверждение регистрации для '+request.params.login,
					type:' multipart/related',
					date:new Date(),
				},
				[
					Mime.Part(
						{	type:'multipart/alternative',},
						[	Mime.Part(
								{type:'text/plain; charset=UTF-8',
								encoding:'quoted-printable',},
								'подтверди по ссылке '+confirmHref
							),
							Mime.Part(
								{type:'text/html; charset=UTF-8',
								encoding:'base64',},
								'подтверди по ссылке <a href="'+confirmHref+'">'+confirmHref+'</a>'
							),
						]
					),
				]
			);


			let smtp = new Net.Smtp();
			smtp.host = router.getConfig().smtp.host;
			smtp.port = router.getConfig().smtp.port;
			smtp.login = router.getConfig().smtp.login;
			smtp.password = router.getConfig().smtp.password;


			smtp.send(msg);
		}
		//ui.blocks.center.push(this.properties.registerConfirmation.render(request.params.email));

		router.cd.session.userId = user.id;
		global.user = user;

		router.cd.global.session.remember = false;
		request.pushHeader('Set-Cookie', 'sid='+router.cd.global.session.id+'; path=/');
		request.setStatusCode(303);
		request.pushHeader('Location', request.params.backUrl?request.params.backUrl:'/forum');

		ui.print();
		return;
		

	}
}
catch(e)
{
	delete this.cd.session.captcha;
	this.msg = String(e);
}

ui.blocks.center.push(this.render());
ui.print();
