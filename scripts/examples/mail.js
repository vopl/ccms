	let msg = new Mime.Part(
		{	from:Mime.HeaderAddress('peltup@bk.ru'),
			to:'<vopl@bk.ru>, <shtoba@bk.ru>',
			cc:'мой ящ"ик <spamreceptor@mail.ru>, не мой <peltup@bk.ru>',
			subject:'Тратата тема нах',
			type:' multipart/related',
			date:new Date(),
		},
		[
			Mime.Part(
				{	type:'multipart/alternate',},
				[	Mime.Part(
						{type:'text/plain; charset=UTF-8',
						encoding:'base64',},
						'тут содержимое части 1'
					),
					Mime.Part(
						{type:'text/html; charset=UTF-8',
						encoding:'base64',},
						'<b>тут содержимое части 2</b> <img src="cid:img.jpg"/>'
					),
					Mime.Part(
						{type:'text/html; charset=UTF-8',
						encoding:'quoted-printable',},
						'<b>тут содержимое части 3</b> <img src="cid:img.jpg"/>'
					),
				]
			),
			Mime.Part(
				{	type:'image/jpg',
					encoding:'base64',
					disposition:'inline; filename=img.jpg',
					id:'img.jpg'
				},
				Mime.File('C:/temp/1.JPG')
			)
		]
	);


let smtp = new Net.Smtp();
	smtp.host = 'localhost';
	smtp.port = 25;
	// smtp.login = 'peltup@bk.ru';
	// smtp.password = 'sdf';


smtp.send(msg);
