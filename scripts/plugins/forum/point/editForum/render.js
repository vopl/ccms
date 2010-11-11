let forum = arguments[0];
if(!forum) forum = {};

function levelHelper(level)
{
	let res = <></>;
	for(let i=0; i<level; i++) res += <>&#160;&#160;</>;

	return res;
}




return <>
<form method="POST">
	<input type='hidden' name='save' value='1'/>
	<input type='hidden' name='id' value={forum.id?forum.id:''}/>
	tree_pid:<select name="tree_pid">
	
		{(function(){
			let forums;
			
			if(forum.id)
			{
				//кроме самого и дочерних
				forums = orm.query('SELECT id, tree_pid, tree_level, map_title FROM {Forum} WHERE tree_left<$1 OR tree_right>$2 OR tree_root!=$3 ORDER BY tree_left', forum.tree_left, forum.tree_right, forum.tree_root);
			}
			else
			{
				//все
				forums = orm.query('SELECT id, tree_pid, tree_level, map_title FROM {Forum} ORDER BY tree_left');
			}
			let result = <></>;
			for each(let pforum in forums)
			{
				let option = <option value={pforum.id}>{levelHelper(pforum.tree_level+1)}&#183;{pforum.map_title}</option>;
				if(pforum.id == forum.tree_pid)
				{
					option.@selected=1;
				}
				result += option;
			}
			return result;
		})()}
	</select><br/>
	map_title:<input type='string' name='map_title' value={forum.map_title?forum.map_title:''}/><br/>
	map_path:<input type='string' name='map_path' value={forum.map_path?forum.map_path:''}/><br/>
	
	description:<input type='string' name='description' value={forum.description?forum.description:''}/><br/>
	
	allow_topic:<input type='checkbox' name='allow_topic' value='1' {forum.allow_topic?'checked':'data-checked'}='1'/><br/>
	<input type='submit'/>
</form>

</>
