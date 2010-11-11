
if(request.planData.mode && request.planData.mode == 'navigate')
{
	return this.properties.renderNavigate();
}

let target = request.planData.forums[request.planData.forums.length-1];
return this.properties.renderTopics(target);
