function checkNsTree(data, level, correct, element)
{
	if(element)
	{
		if(element.tree_level != level)
		{
			if(correct)
			{
				orm.exec('UPDATE {NsTree} SET tree_level=$1 WHERE id=$2', level, element.id);
			}
			else
			{
				dumpe(['tree_level', level, element]);
				return false;
			}
		}
	
	
		if(element.tree_left <= data.cnt)
		{
			if(correct)
			{
				orm.exec('UPDATE {NsTree} SET tree_left=$1 WHERE id=$2', data.cnt+1, element.id);
				element.tree_left = data.cnt+1;
			}
			else
			{
				dumpe(['tree_left', data, element]);
				return false;
			}
		}
		data.cnt = element.tree_left;
	
	
	
	
		let childs = orm.query('SELECT * FROM {NsTree} WHERE tree_pid=$1 ORDER BY tree_left', element.id);
		for each(let child in childs)
		{
			if(!checkNsTree(data, level+1, correct, child)) return false;
		}
		
		if(element.tree_right <= data.cnt)
		{
			if(correct)
			{
				orm.exec('UPDATE {NsTree} SET tree_right=$1 WHERE id=$2', data.cnt+1, element.id);
				element.tree_right = data.cnt+1;
			}
			else
			{
				dumpe(['tree_right', data, element]);
				return false;
			}
		}
		data.cnt = element.tree_right;
		
	}
	else
	{
		//выбрать корни
		let roots = orm.query('SELECT * FROM {NsTree} WHERE tree_pid IS NULL ORDER BY tree_left');
		for each(let root in roots)
		{
			data.cnt=-100000;
			if(!checkNsTree(data, level, correct, root)) return false;
		}
	}
	
	return true;
}
 
return <>{checkNsTree({cnt:-100000}, 0, request.params.c)}</>;