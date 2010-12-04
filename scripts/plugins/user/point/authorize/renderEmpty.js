let t = arguments.callee.t;
if(!t)
{
	let submitUrl = request.env.host || "";
	if(submitUrl.match(/.*?\:\d+$/))
	{
		submitUrl = submitUrl.replace(/\:(\d+)/, ':'+request.env.SERVER_PORTSSL);
	}
	else
	{
		//submitUrl += ':'+request.env.SERVER_PORTSSL;
	}
	submitUrl = "https://"+submitUrl+this.path;

	t = router.createTemplate();
	let xml = <div>
		<div>{t(_('Not authorized'))}</div>
		
		<div>{t.msg}</div>
		<div>
			<form method="POST" action={submitUrl}>
		
				<input type="hidden" name="do" value="login"/>
				<input type="hidden" name="backUrl" value={t.backUrl}/>
		
				<div><input name="login" value={t.login}/></div>
				<div><input type="password" name="password" value={t.password}/></div>
				<div>
					<input id={this.id+'_remember'} name="remember" type="checkbox" value='1' {t.remember}="1"/>
					<label for={this.id+'_remember'}>{t(_('Remember'))}</label>
					<input type='submit' value="да"/>
				</div>
			</form>
		</div>
		<div>
			<a href={this.parent.childs.register.path}>{t(_('Register'))}</a>
			<a href={this.parent.childs.restorepswd.path}>{t(_('Restore password'))}</a>
		</div>
	</div>;
	t.compile(xml);
	arguments.callee.t = t;
}

t = t.clone();
t.msg = this.msg?<b>{this.msg}</b>:"";
delete this.msg;
t.login = request.params.login;
t.password = request.params.password;
t.remember = request.params.remember?"checked":"data-checked";
t.backUrl = request.params.backUrl?request.params.backUrl:request.path;

return t;
