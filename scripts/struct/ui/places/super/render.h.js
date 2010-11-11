let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	let xml = <>
		{t.pre}

		{t(function(){
			return t.obj.self.properties.pre.render();
		})}
		
		{t.preContent}
		{t(function(){
			let self = t.obj.self;
			if('data' in self) return self.data;
			return self.properties.data();
		})}
		{t.postContent}

		{t(function(){
			return t.obj.self.properties.post.render();

		})}

		{t.post}
	</>;
	t.compile(xml);
	arguments.callee.t=t;
}

let hooks = arguments.callee.hooks;
if(!hooks)
{
	hooks = {};
	arguments.callee.hooks = hooks;
}



let name = this.properties.hookId;
if(!hooks[name])
{
	hooks[name] = 
	{
		pre:ui.hookPlace(name, "pre"),
		preContent:ui.hookPlace(name, "preContent"), 
		postContent:ui.hookPlace(name, "postContent"), 
		post:ui.hookPlace(name, "post"),
	};
}
hooks = hooks[name];



t = t.clone();
t.self = this;
t.pre = hooks.pre;
t.preContent = hooks.preContent;
t.postContent = hooks.postContent;
t.post = hooks.post;


return t;


