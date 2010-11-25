let mostForum = request.planData.forums.back;
let res = mostForum.map_title?mostForum.map_title:('forum #'+mostForum.id);

return res;
