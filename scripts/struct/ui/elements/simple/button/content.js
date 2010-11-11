let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<button value={t.value}/>);
	arguments.callee.t = t;
}

t = t.clone();

t.value = arguments[0];

return t;

