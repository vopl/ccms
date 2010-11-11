if(arguments.callee.t) return arguments.callee.t;


let t = router.createTemplate();
t.compile(<div>main content at {this.path}</div>);
arguments.callee.t=t;
return t;
