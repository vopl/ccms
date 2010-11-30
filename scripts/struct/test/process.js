let t = arguments.callee.header;
if(!t)
{
	t = router.createTemplate();
	t.compile(<parent> l {t('auth', '')} ll </parent>);
	arguments.callee.header = t;
}
t = t.clone();
t.auth = <tag>asdf</tag>;
t.print();
return;

ui.blocks.center.push(this.render());
ui.print();

