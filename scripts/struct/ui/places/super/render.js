let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	let xml = <>{t(function(){
			return t.obj.self.properties.pre();
		})}{t(function(){
			return t.obj.self.properties.wrapper(t.obj.self.properties.data());
		})}{t(function(){
			return t.obj.self.properties.post();

		})}</>;

	t.compile(xml);
	arguments.callee.t=t;
}



t = t.clone();
t.self = this;


return t;


