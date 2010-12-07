let t = arguments.callee.header;
if(!t)
{
	t = router.createTemplate();
	t.compile(<parent attr={t('auth', '')}> lll </parent>);
	arguments.callee.header = t;
}
t = t.clone();

ui.blocks.center.push(this.render());
ui.print();

