if(global.user.login != 'anonymous')
{
	return <>already logged in</>;
}

///////////////////////////////////////////////////

let captchaPassword = Crypto.Rand.abc('abcdefghijklmnopqrstuvwxwz', 3);


let captchaCode = Crypto.Rand.str_(22);

this.cd.session.captcha = {};
this.cd.session.captcha[captchaPassword] = captchaCode;


/////////////////////////////////////////////////////////////
let captchaImage = new Magick.Image('130x30' ,'#ffffff');
captchaImage.magick('JPEG');
//captchaImage.attributes().font('arial');
captchaImage.attributes().fontPointsize(20);
captchaImage.annotate(captchaPassword
	.split(/\s*/).join(' '),
	Magick.CenterGravity);

let degress = Crypto.Rand.double(-5, 5);
if(degress < 0) degress -= 5;
else degress += 5;
captchaImage.swirl(degress);

captchaImage.roll(20, 0);
degress = Crypto.Rand.double(-5, 5);
if(degress < 0) degress -= 5;
else degress += 5;
captchaImage.swirl(degress);

captchaImage.roll(-40, 0);
degress = Crypto.Rand.double(-5, 5);
if(degress < 0) degress -= 5;
else degress += 5;
captchaImage.swirl(degress);

captchaImage.crop('90x30');
captchaImage.border('1x1');
///////////////////////////////////////////////////

return <>
		<hr/>
	соглашение
	<p>бла бла бла бла бла </p>
	
	<form method='POST'>
		<input type='hidden' name='save' value='1'/>
		принимаю <input type='checkbox' name='acceptAgreement' value='1' {request.params.acceptAgreement?'checked':'data-checked'}="1"/><br/>
		<hr/>
		логин<input type='string' name='login' value={request.params.login || ''}/><br/>
		пароль<input type='password' name='password' value={request.params.password || ''}/><br/>
		пароль<input type='password' name='password2' value={request.params.password2 || ''}/><br/>

		почта<input type='string' name='email' value={request.params.email || 'keep.me@example.com'}/> не используется, можно оставить этот фиктивный адрес<br/>
		
		<div style="display: none">
			капча 
			<input type='hidden' name='captchaCode' value={captchaCode}/><br/>
			<img src={'data:image/jpeg;base64,'+captchaImage.getBase64()}/> 
			<input type='string' name='captchaPassword' value={captchaPassword}/><br/>
		</div>
		<input type='submit' value="да"/>
	</form>
	<strong style="color: red">{this.msg}</strong>
</>;
