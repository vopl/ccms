let post = arguments[0] || request.planData.post;

if(acl.hasRight('forum', 'managePosts')) return true;

if(request.planData.mode && request.planData.mode=='add')
{
	return acl.hasRight('forum', 'writePost');
}

if(!post)
{
	return acl.hasRight('forum', 'writePost');
}


if('anonymous' != user.login)
{
	if(post.user_id == user.id) return true;
}

return false;
