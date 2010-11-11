let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	let xml = <div>
		{t(_('Login'))}:{t.login}<br/>
		<a href={t.exitUrl}>{t(_('Exit'))}</a>
		<a href={t.cabinetUrl}>{t(_('Cabinet'))}</a>
	</div>;
	t.compile(xml);
	arguments.callee.t = t;
}

t = t.clone();
t.login = user.login;
t.exitUrl = this.path+'?do=logout&backUrl='+(request.params.backUrl?request.params.backUrl:request.path);
t.cabinetUrl = this.parent.childs.cabinet.path;

return t;
