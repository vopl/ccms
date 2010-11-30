let t = arguments.callee.t;

if(!t)
{
	t = router.createTemplate();
	t.compile(<a href={t.href}>{t.title}</a>)
	arguments.callee.t = t;
}

t = t.clone();

t.href = arguments[0];
t.title = arguments[1];
if(!t.title) t.title = t.href;

return t;
