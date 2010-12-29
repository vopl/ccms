this.properties.ensureRootForum(request.planData);
let forum = request.planData.forum;
while(forum.parent) forum = forum.parent;
return forum.title?forum.title:'Корень форумов';
