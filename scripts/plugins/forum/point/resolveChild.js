let pathPart = arguments[0];
let data = arguments[1];

this.properties.ensureRootForum(data);

let rootForum = data.forum;
return { 
	goto:this.childs.viewForum, 
	title:rootForum.title,
};
