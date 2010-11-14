if(!request.params.login)
{
	return;
}
if(request.params.send)
{
	let user = orm.User.select('WHERE login=$1', request.params.login)[0];
	if(user)
	{
		let s = /waitConfirm:(.*)/.exec(user.status);
		if(s && s[1])
		{
		
		
			let confirmSecret = s[1];
		
			//отправить письмо
			{
				let confirmHref = 'http://'+request.env.host+'/user/register/confirm?secret='+confirmSecret;
				let msg = new Mime.Part(
					{	from:Mime.HeaderAddress('spamreceptor@mail.ru'),
						to:user.email,
						subject:'Подтверждение регистрации для '+user.login,
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
				smtp.host = 'localhost';
				smtp.port = 25;
				// smtp.login = 'peltup@bk.ru';
				// smtp.password = 'sdf';


				smtp.send(msg);
			}		
		
		
		
		
			
			ui.blocks.center.push(this.properties.renderSended());
			ui.print();
			return;
		}
	}
	ui.blocks.center.push(this.properties.renderBad());
	ui.print();
	return;
}
else
{
	ui.blocks.center.push(this.render());
	ui.print();
	return;
}
