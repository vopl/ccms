let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(t.pre+t.prolog+t.preContent+t.content+t.postContent+t.epilog+t.post);
	arguments.callee.t = t;
}

let hooks = arguments.callee.hooks;
if(!hooks)
{
	hooks = {};
	arguments.callee.hooks = hooks;
}



let name = this.properties.name;
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

t.pre = hooks.pre;
t.prolog = this.properties.prolog();
t.preContent = hooks.preContent;
t.content = this.properties.content();
t.postContent = hooks.postContent;
t.epilog = this.properties.epilog();
t.post = hooks.post;

return t;

