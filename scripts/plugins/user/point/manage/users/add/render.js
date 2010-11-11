let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<>
	add user
	</>);
	arguments.callee.t=t;
}


return t;
