let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<a href={t.href}>{t.name}</a>);
	arguments.callee.t = t;
}

t = t.clone();

t.href = arguments[0];
t.name = arguments[1];

return t;

