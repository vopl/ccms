if(acl.hasRight('forum', 'managePosts')) return true;

if(!request.planData.posts || !request.planData.posts.length)
{
	return acl.hasRight('forum', 'writePost');
}


let mostPost = request.planData.posts.back;
if(mostPost.user_id == user.id) return true;

return false;
