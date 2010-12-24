if(acl.hasRight('forum', 'managePosts')) return true;

if(request.planData.mode && request.planData.mode=='add')
{
	return acl.hasRight('forum', 'writePost');
}

if(!request.planData.post)
{
	return acl.hasRight('forum', 'writePost');
}


if('anonymous' != user.login)
{
	let mostPost = request.planData.post;
	if(mostPost.user_id == user.id) return true;
}

return false;
