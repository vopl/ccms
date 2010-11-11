this.properties.ensureRootForum(request.planData);

let rootForum = request.planData.forums[0];
return rootForum.map_title?rootForum.map_title:('forum #'+rootForum.id);
