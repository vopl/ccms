let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(
		<div class={t.class}>
			{t.content}
		</div>);
	arguments.callee.t = t;
}

t = t.clone();
t.content = arguments[0];
t.class = arguments[1];

return t;