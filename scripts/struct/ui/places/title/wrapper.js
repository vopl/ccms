
let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	let xml = <title>{t.content}</title>;
	t.compile(xml);
	arguments.callee.t=t;
}



t = t.clone();
t.content = arguments[0];


return t;
