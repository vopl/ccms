
if(request.params.id)
{
	orm.User.delete({id:request.params.id});
	if(request.params.backUrl)
	{
		request.setStatusCode(303);
		request.pushHeader('Location', request.params.backUrl?request.params.backUrl:'/');
		return;
	}
}
