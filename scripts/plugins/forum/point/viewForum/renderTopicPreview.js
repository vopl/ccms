let topic = arguments[0];

let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<span style="background: #c0c0c0;"><a href={t.path}>{t.title}</a> {t.time} {t.user}</span>);
	arguments.callee.t = t;
}
t = t.clone();

t.path = topic.path;
t.title = topic.title;
t.time = topic.ctime.ts;
t.user = topic.user.login;

return t;