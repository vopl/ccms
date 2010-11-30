let t = arguments.callee.header;
if(!t)
{
	t = router.createTemplate();
	t.compile(<>{t.auth}</>);
	t.auth = 'original';//router.getPoint('/user/authorize');
	arguments.callee.header = t;
}
t = t.clone();
t.auth = 'replacement';//t.auth.render();
t.print();
return;

ui.blocks.center.push(this.render());
ui.print();

