if(global.user.login != 'anonymous')
{
	return <>already logged in</>;
}

///////////////////////////////////////////////////
let captchaPassword = '';
for(let i=0; i<3; i++)
{
	captchaPassword += String.fromCharCode('a'.charCodeAt(0) + Math.floor(Math.random()*24));
}

let captchaCode = String(Math.random());

this.cd.session.captcha = {};
this.cd.session.captcha[captchaPassword] = captchaCode;


/////////////////////////////////////////////////////////////
let captchaImage = new Magick.Image('130x30' ,'#ffffff');
captchaImage.magick('JPEG');
//captchaImage.attributes().font('arial');
captchaImage.attributes().fontPointsize(20);
captchaImage.annotate(captchaPassword
	.split(/\s*/).join(' ')
	, Magick.CenterGravity);

let degress = (Math.random()*2-1)*5;
if(degress < 0) degress -= 5;
else degress += 5;
captchaImage.swirl(degress);

captchaImage.roll(20, 0);
degress = (Math.random()*2-1)*5;
if(degress < 0) degress -= 5;
else degress += 5;
captchaImage.swirl(degress);

captchaImage.roll(-40, 0);
degress = (Math.random()*2-1)*5;
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
		почта<input type='string' name='email' value={request.params.email || ''}/><br/>
		
		
		капча 
			<input type='hidden' name='captchaCode' value={captchaCode}/><br/>
			<img src={'data:image/jpeg;base64,'+captchaImage.getBase64()}/> 
			<input type='string' name='captchaPassword'/><br/>
		<input type='submit' value="да"/>
	</form>
	{this.msg}
</>;
