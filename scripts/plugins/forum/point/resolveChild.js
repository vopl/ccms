let pathPart = arguments[0];
let data = arguments[1];

this.properties.ensureRootForum(data);

let rootForum = data.forums[0];
return { 
	goto:this.childs.viewForum, 
	title:rootForum.title,
};
