let t = arguments.callee.header;
if(!t)
{
	t = router.createTemplate();
	t.compile(<parent attr={t('auth', '')}> lll </parent>);
	arguments.callee.header = t;
}
t = t.clone();
t.auth = {print:function(){print(<tag>asdf</tag>.toXMLString())}};
t.print();
return;

ui.blocks.center.push(this.render());
ui.print();

