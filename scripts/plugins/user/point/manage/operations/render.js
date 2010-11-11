if(arguments.callee.t) return arguments.callee.t;


let t = router.createTemplate();
t.compile(<div>
main content at {this.path}
<a href={this.childs.users.path}>пользователи</a>
<a href={this.childs.roles.path}>роли</a>
<a href={this.childs.operations.path}>операции</a>
<pre>
тут дерево пользователей
дерево ролей
дерево операций

добавить редактировать удалить XX пользователя роль операцию
</pre>

</div>);
arguments.callee.t=t;
return t;
