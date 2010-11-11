let data = arguments[1];

this.properties.ensureRootForum(data);

let rootForum = data.forums[0];
return { 
	point:this.childs.viewForum, 
	title:rootForum.map_title?rootForum.map_title:('forum #'+rootForum.id),
};
