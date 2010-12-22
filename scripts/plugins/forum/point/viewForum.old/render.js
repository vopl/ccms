
let t = arguments.callee.t;

if(!t)
{
	t = router.createTemplate();
	
	t.compile(<div class="forum">
	
		<div class="forum-left">
			{t.logo}
			{t.moders}
		</div>
		<div class="forum-center">
			{t.header}
			{t.topicNavigate}
			{t.navigate}
			{t.footer}
		</div>
	</div>);
	arguments.callee.t = t;
}

t = t.clone();


t.logo = this.properties.renderLogo();
t.moders = this.properties.renderModers();
t.header = this.properties.renderHeader();
t.navigate = this.properties.renderNavigate();
t.topicNavigate = this.properties.renderTopicNavigate();
t.footer = this.properties.renderFooter();

return t;

