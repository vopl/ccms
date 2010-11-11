
let t = arguments.callee.t;

if(!t)
{
	t = router.createTemplate();
	t.compile(<div>
	{t.usersLink}
	{t.rolesLink}
	<pre>
	тут дерево пользователей
	дерево ролей
	дерево операций

	добавить редактировать удалить XX пользователя роль операцию
	</pre>
	{t(ui.skin.link('href', 'title'))}
	</div>);
	arguments.callee.t=t;
}

t = t.clone();






let manageUsers = acl.hasRight('user', 'manageUsers');
let manageRoles = acl.hasRight('user', 'manageRoles');

t.usersLink = manageUsers?ui.skin.link(this.childs.users.path, 'пользователи'):'';
t.rolesLink = manageRoles?ui.skin.link(this.childs.roles.path, 'роли'):'';


return t;
