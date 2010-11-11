let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(t.prolog+t.content+t.epilog);
	arguments.callee.t = t;
}




t = t.clone();

t.prolog = this.properties.prolog();
t.content = this.properties.content();
t.epilog = this.properties.epilog();

return t;

