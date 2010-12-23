let target = arguments[0];
if(!target) target = request.planData.forum;
let point = this;

let t = arguments.callee.t;

if(!t)
{
	t = router.createTemplate();
	
	t.compile(<div class="forum">
	
		<div class="forum-header">{t.header}</div>
		<div class="forum-center">{t.center}</div>
		<div class="forum-footer">{t.footer}</div>
	</div>);
	arguments.callee.t = t;
}

t = t.clone();


t.header = this.properties.renderHeader(target);
t.center = this.properties.renderCenter(target);
t.footer = this.properties.renderFooter(target);

return t;

