let mostForum = request.planData.forums.back;
let res = mostForum.map_title?mostForum.map_title:('forum #'+mostForum.id);

if(request.planData.mode && request.planData.mode=="navigate")
{
	res = 'navigate '+res;
}

return res;
