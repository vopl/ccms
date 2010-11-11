var orm = arguments[0];
var install = arguments[1];

if(!install)
{
	orm.delCategory('NsTree');
	return;
}








let step = 10;





///////////////////////////////////////////////////////////////////
function checkNsTree(category, data, level, correct, element)
{
	if(element)
	{
		if(element.tree_level != level)
		{
			if(correct)
			{
				orm.exec('UPDATE {'+category.name+'} SET tree_level=$1 WHERE id=$2', level, element.id);
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
				orm.exec('UPDATE {'+category.name+'} SET tree_left=$1 WHERE id=$2', data.cnt+1, element.id);
				element.tree_left = data.cnt+1;
			}
			else
			{
				dumpe(['tree_left', data, element]);
				return false;
			}
		}
		data.cnt = element.tree_left;
	
	
	
	
		let childs = orm.query('SELECT * FROM {'+category.name+'} WHERE tree_pid=$1 ORDER BY tree_left', element.id);
		for each(let child in childs)
		{
			if(!checkNsTree(category, data, level+1, correct, child)) return false;
		}
		
		if(element.tree_right <= data.cnt)
		{
			if(correct)
			{
				orm.exec('UPDATE {'+category.name+'} SET tree_right=$1 WHERE id=$2', data.cnt+1, element.id);
				element.tree_right = data.cnt+1;
			}
			else
			{
				dumpe(['tree_right', data, element]);
				return false;
			}
		}
		
		if(element.tree_root != data.root)
		{
			if(correct)
			{
				orm.exec('UPDATE {'+category.name+'} SET tree_root=$1 WHERE id=$2', data.root, element.id);
			}
			else
			{
				dumpe(['tree_root', data.root, element]);
				return false;
			}
		}
		
		data.cnt = element.tree_right;
		
	}
	else
	{
		//выбрать корни
		let roots = orm.query('SELECT * FROM {'+category.name+'} WHERE tree_pid IS NULL ORDER BY tree_left');
		for each(let root in roots)
		{
			data = {};
			data.cnt=-100000;
			data.root = root.id;
			if(!checkNsTree(category, data, 0, correct, root)) return false;
		}
	}
	
	return true;
}

/////////////////////////////////////////////////////////////////////////
function asTrans(category, func)
{
	let ownTrans = ('IDLE' == db.transactionStatus);
	
	if(ownTrans) db.exec('BEGIN');
	else db.exec('SAVEPOINT nstree_manipulation');
	
	try
	{
		func();
		if(!checkNsTree(category)) throw '!checkNsTree';
		
		if(ownTrans) db.exec('COMMIT');
		else db.exec('RELEASE SAVEPOINT nstree_manipulation');
	}
	catch(e)
	{
		if(ownTrans) db.exec('ROLLBACK');
		else db.exec('ROLLBACK TO SAVEPOINT nstree_manipulation');
		
		throw e;
	}
}






/*
	вставка нового корня
	
	так как множества у каждого дерева свои то не нужно выравнивать 
	относительно других корней. Тупо вставить около нуля
*/
function insertRootImpl(category, v)
{
	v.id = undefined;
	v.tree_pid = undefined;
	v.tree_left = 0;
	v.tree_right = step;
	v.tree_level = 0;

	category.getImpl().insert(v);
	orm.exec('UPDATE {'+category.name+'} SET tree_root=$1 WHERE id=$1', v.id);
	v = category.load(v);
	
	if(!checkNsTree(category, {cnt:-100000}, 0, false)) throw Error("wrong state");

	return v;
}

/*
	вставка дочернего элемента
	
	его надо позиционировать относительно сестринских элементов
*/
function insertChildImpl(category, v)
{
	//выбрать родителя, у него взять признак дерева, левые правые границы, уровень
	let dbr = orm.query(
		'SELECT \
			parent.id, \
			parent.tree_left AS parent_left, \
			parent.tree_right AS parent_right, \
			parent.tree_root AS tree_root, \
			parent.tree_level AS parent_level \
		FROM \
			{'+category.name+'} AS parent \
		WHERE parent.id=$1', v.tree_pid);
	if(dbr.length != 1)
	{
		throw Error('NsTree insertChildImpl: bad pid');
	}
	dbr = dbr[0];
	let parent_left = dbr.parent_left;
	let parent_right = dbr.parent_right;
	let tree_root = dbr.tree_root;
	let parent_level = dbr.parent_level;
	
	//желаемая позиция
	let middle;
	if(v.tree_left !== undefined && v.tree_right !== undefined)
		middle = (v.tree_left + v.tree_right)/2;
	else if(v.tree_left !== undefined)
		middle = v.tree_left;
	else if(v.tree_right !== undefined)
		middle = v.tree_right;
		
	let lce, rce;
	if(middle !== undefined)
	{
		//в желаемую позицию
		if(middle > parent_right) middle = parent_right-0.5;
		if(middle < parent_left) middle = parent_left+0.5;
		
		//левые
		lce = orm.query('SELECT tree_right AS value FROM {'+category.name+'} WHERE tree_pid=$1 AND tree_right<=$2 ORDER BY tree_right DESC LIMIT 1', v.tree_pid, middle)[0];
		//правые
		rce = orm.query('SELECT tree_left AS value FROM {'+category.name+'} WHERE tree_pid=$1 AND tree_left>=$2 ORDER BY tree_left ASC LIMIT 1', v.tree_pid, middle)[0];
	}
	else
	{
		lce = orm.query('SELECT tree_right AS value FROM {'+category.name+'} WHERE tree_pid=$1 ORDER BY tree_right DESC LIMIT 1', v.tree_pid)[0];
		rce = {value:parent_right};
	}
	if(!lce) lce = {value:parent_left};
	if(!rce) rce = {value:parent_right};

	//и левые и правые есть
	let distance = rce.value - lce.value;
	if(distance<3)
	{
		orm.exec('UPDATE {'+category.name+'} SET tree_left=tree_left+$1 WHERE tree_left >= $2 AND tree_root=$3', step*2, rce.value, tree_root);
		orm.exec('UPDATE {'+category.name+'} SET tree_right=tree_right+$1 WHERE tree_right >= $2 AND tree_root=$3', step*2, rce.value, tree_root);
		rce.value += step;
		distance = rce.value - lce.value;
	}
	
	//вдруг граница уже удовлетворяет условиям
	if(v.tree_left !== undefined && v.tree_right !== undefined)
	{
		if(v.tree_left>=v.tree_right) v.tree_right = v.tree_left+1;
		if(v.tree_left <= lce.value) v.tree_left = lce.value+1;
		if(v.tree_right >= rce.value) v.tree_left = rce.value-1;
	}
	else if(v.tree_left !== undefined)
	{
		if(v.tree_left <= lce.value) v.tree_left = lce.value+1;
		v.tree_right = v.tree_left+1;
	}
	else if(v.tree_right !== undefined)
	{
		if(v.tree_right >= rce.value) v.tree_right = rce.value-1;
		v.tree_left = v.tree_right-1;
	}
	else
	{
		v.tree_left = Math.round(lce.value+distance/3);
		v.tree_right = Math.round(rce.value-distance/3);
	}
	
	v.tree_level = parent_level+1;
	v.tree_root = tree_root;
	category.getImpl().insert(v);
}

/*
	обновление корня
	
	корректировать границы по дочерним
	даже если узел ранее был дочерним у когото другого
	то корректировать дочерних не нужно, так как новое дерево - новое множество
*/
function updateRootImpl(category, v)
{
	let dbr = orm.query(
		'SELECT \
			self.id, \
			self.tree_pid AS tree_pid, \
			self.tree_level AS tree_level \
		FROM \
			{'+category.name+'} AS self \
		WHERE self.id=$1', v.id);
	if(dbr.length != 1)
	{
		throw Error('NsTree updateRootImpl: bad id');
	}
	
	let oldPid = dbr[0].tree_pid;
	let oldLevel = dbr[0].tree_level;
	
	//выявить диапазон дочерних
	let lce, rce, hasChilds;
	//левые (заодно взять признак дерева, пригодится при обновлении дочерних)
	lce = orm.query('SELECT tree_left AS value, tree_root FROM {'+category.name+'} WHERE tree_pid=$1 ORDER BY tree_left LIMIT 1', v.id)[0];
	if(lce)
	{
		hasChilds = true;
		//раз левые есть то и правые есть
		//правые
		rce = orm.query('SELECT tree_right AS value FROM {'+category.name+'} WHERE tree_pid=$1 ORDER BY tree_right DESC LIMIT 1', v.id)[0];
		
		if(v.tree_left !== undefined)
		{
			if(v.tree_left >= lce.value)
				v.tree_left = lce.value - step;
		}
		else
		{
			v.tree_left = lce.value - step;
		}

		if(v.tree_right !== undefined)
		{
			if(v.tree_right <= rce.value)
				v.tree_right = rce.value + step;
		}
		else
		{
			v.tree_right = rce.value + step;
		}
	}
	else
	{
		hasChilds = false;
		//дочерних нет
		if(v.tree_left !== undefined)
		{
			if(v.tree_right !== undefined)
			{
				if(v.tree_left >= v.tree_right) v.tree_right = v.tree_left + step;
			}
			else
			{
				v.tree_right = v.tree_left + step;
			}
		}
		else
		{
			if(v.tree_right !== undefined)
			{
				v.tree_left = v.tree_right - step;
			}
			else
			{
				v.tree_left = 0;
				v.tree_right = step;
			}
		}
	}
	v.tree_level = 0;
	v.tree_root = v.id;
	category.getImpl().update(v);
	
	if(oldPid && hasChilds)
	{
		//дочерним скорректировать уровень, признак дерева
		
		//взять уровень и признак старого дерева у первого попавшегося дочернего
		let oldRoot = lce.tree_root;
		let levelCorrection = v.tree_level - oldLevel;
		
		if(oldRoot != v.id && levelCorrection !=0 )
		{
			orm.exec('UPDATE {'+category.name+'} \
				SET \
					tree_root=$1, \
					tree_level=tree_level+$2 \
				WHERE \
					tree_root=$3 AND \
					tree_left>=$4 AND \
					tree_right<=$5',
				v.id, levelCorrection, oldRoot, lce.value, rce.value);
		}
		else if(oldRoot != v.id)
		{
			orm.exec('UPDATE {'+category.name+'} \
				SET \
					tree_root=$1, \
				WHERE \
					tree_root=$2 AND \
					tree_left>=$3 AND \
					tree_right<=$4',
				v.id, oldRoot, lce.value, rce.value);
		}
		else if(levelCorrection !=0)
		{
			orm.exec('UPDATE {'+category.name+'} \
				SET \
					tree_level=tree_level+$1 \
				WHERE \
					tree_root=$2 AND \
					tree_left>=$3 AND \
					tree_right<=$4',
				levelCorrection, oldRoot, lce.value, rce.value);
		}
		else
		{
			//ничего корректировать не нужно
		}
	}
}

function updateChildImpl(category, v)
{
	let dbr = orm.query(
		'SELECT \
			self.id, \
			self.tree_pid AS tree_pid, \
			self.tree_root AS tree_root, \
			self.tree_level AS tree_level, \
			self.tree_left AS tree_left, \
			self.tree_right AS tree_right, \
			parent.id AS tree_pid, \
			parent.tree_left AS parent_left, \
			parent.tree_right AS parent_right, \
			parent.tree_root AS parent_root, \
			parent.tree_level AS parent_level \
		FROM \
			{'+category.name+'} AS self, \
			{'+category.name+'} AS parent \
		WHERE \
			self.id=$1 AND parent.id=$2',
		v.id, v.tree_pid);
	if(dbr.length != 1)
	{
		throw Error('NsTree update: bad id/pid');
	}
	dbr = dbr[0];
	
	let oldPid = dbr.tree_pid;
	let oldRoot = dbr.tree_root;
	let oldLevel = dbr.tree_level;
	let oldLeft = dbr.tree_left;
	let oldRight = dbr.tree_right;
	let parentLeft = dbr.parent_left;
	let parentRight = dbr.parent_right;
	let parentRight = dbr.parent_right;
	let parentRoot = dbr.parent_root;
	let parentLevel = dbr.parent_level;
	
	
	if(v.tree_left === undefined) v.tree_left = oldLeft;
	if(v.tree_right === undefined) v.tree_right = oldRight;
	
	
	
	
	
	
	
	
	
	
	let childsTreeRoot;
	
	
	//выявить диапазон дочерних
	let lce, rce, hasChilds;
	//левые (заодно взять признак дерева, пригодится при обновлении дочерних)
	lce = orm.query('SELECT tree_left AS value, tree_root FROM {'+category.name+'} WHERE tree_pid=$1 ORDER BY tree_left LIMIT 1', v.id)[0];
	if(lce)
	{
		hasChilds = true;

		//раз левые есть то и правые есть
		//правые
		rce = orm.query('SELECT tree_right AS value FROM {'+category.name+'} WHERE tree_pid=$1 ORDER BY tree_right DESC LIMIT 1', v.id)[0];
		
		if(v.tree_left !== undefined)
		{
			if(v.tree_left >= lce.value)
				v.tree_left = lce.value - step;
		}
		else
		{
			v.tree_left = lce.value - step;
		}

		if(v.tree_right !== undefined)
		{
			if(v.tree_right <= rce.value)
				v.tree_right = rce.value + step;
		}
		else
		{
			v.tree_right = rce.value + step;
		}
		
		//вывести дочерних из дерева на время перемещений
		childsTreeRoot = category.getNextInsertId();
		orm.exec('UPDATE {'+category.name+'} SET tree_root=$1 WHERE tree_left>=$2 AND tree_right<=$3 AND tree_root=$4', childsTreeRoot, lce.value, rce.value, oldRoot);
	}
	else
	{
		hasChilds = false;
		//дочерних нет
		if(v.tree_left !== undefined)
		{
			if(v.tree_right !== undefined)
			{
				if(v.tree_left >= v.tree_right) v.tree_right = v.tree_left + step;
			}
			else
			{
				v.tree_right = v.tree_left + step;
			}
		}
		else
		{
			if(v.tree_right !== undefined)
			{
				v.tree_left = v.tree_right - step;
			}
			else
			{
				v.tree_left = 0;
				v.tree_right = step;
			}
		}
	}
	


/*
	let oldRoot = dbr.old_root;
	let oldLevel = dbr.old_level;
	let oldLeft = dbr.tree_left;
	let oldRight = dbr.tree_right;
	let parentLeft = dbr.parent_left;
	let parentRight = dbr.parent_right;
*/	
	//выявить диапазон сестринских
	let middle = Math.round((v.tree_left+v.tree_right)/2);
	let lse, rse;
	lse = orm.query('SELECT tree_right AS value, id FROM {'+category.name+'} WHERE tree_pid=$1 AND tree_left<$2 ORDER BY tree_right DESC LIMIT 1', v.tree_pid, middle)[0];
	rse = orm.query('SELECT tree_left AS value, id FROM {'+category.name+'} WHERE tree_pid=$1 AND tree_right>$2 ORDER BY tree_left ASC LIMIT 1', v.tree_pid, middle)[0];

	if(!lse) lse = {value:parentLeft};
	if(!rse) rse = {value:parentRight};
	
	if(lse.id === rse.id && lse.id !== undefined)
	{
		rse ={value:lse.value+1};
	}
	
	if(lse.value >= v.tree_left)
	{
		let move = lse.value - v.tree_left + 2*step;
		orm.exec('UPDATE {'+category.name+'} SET tree_left=tree_left-$1 WHERE tree_left <= $2 AND tree_root=$3', move, lse.value, parentRoot);
		orm.exec('UPDATE {'+category.name+'} SET tree_right=tree_right-$1 WHERE tree_right <= $2 AND tree_root=$3', move, lse.value, parentRoot);
		lse.value -= move;
	}
	
	if(rse.value <= v.tree_right)
	{
		let move = v.tree_right - rse.value + 2*step;
		orm.exec('UPDATE {'+category.name+'} SET tree_left=tree_left+$1 WHERE tree_left >= $2 AND tree_root=$3', move, rse.value, parentRoot);
		orm.exec('UPDATE {'+category.name+'} SET tree_right=tree_right+$1 WHERE tree_right >= $2 AND tree_root=$3', move, rse.value, parentRoot);
		rse.value += move;
	}
	
	v.tree_level = parentLevel+1;
	v.tree_root = parentRoot;
	category.getImpl().update(v);
	
	if(oldPid && hasChilds)
	{
		//дочерним скорректировать уровень, признак дерева
		
		//взять уровень и признак старого дерева у первого попавшегося дочернего
		let levelCorrection = v.tree_level - oldLevel;
		
		if(levelCorrection !=0 )
		{
			orm.exec('UPDATE {'+category.name+'} \
				SET \
					tree_root=$1, \
					tree_level=tree_level+$2 \
				WHERE \
					tree_root=$3',
				parentRoot, levelCorrection, childsTreeRoot);
		}
		else
		{
			orm.exec('UPDATE {'+category.name+'} \
				SET \
					tree_root=$1 \
				WHERE \
					tree_root=$2',
				parentRoot, childsTreeRoot);
		}
	}	
	
}








function moveImpl(category, v, bound)
{
	//выявить границы цели
	let dbr = orm.query(
	'SELECT \
		tree_pid, \
		tree_left, \
		tree_right, \
		tree_root \
	FROM \
		{'+category.name+'} \
	WHERE id=$1', v.id);
	
	if(dbr.length != 1)
	{
		throw Error('NsTree move: bad value');
	}
	
	let treePid = dbr[0].tree_pid;
	let targetLeft = dbr[0].tree_left;
	let targetRight = dbr[0].tree_right;
	let treeRoot = dbr[0].tree_root;

	let moveLeft;
	if(bound < targetLeft)
	{
		//двигать влево
		moveLeft = true;
	}
	else if(bound > targetRight)
	{
		//двигать вправо
		moveLeft = false;
	}
	else
	{
		//никуда не двигать
		return v;
	}
	

	//выявить границы перемещаемых узлов
	let concurentLeft, concurentRight;
	if(moveLeft)
	{//влево
		if(treePid)
			dbr = orm.query('SELECT tree_left FROM {'+category.name+'} WHERE tree_pid=$1 AND tree_left>=$2 ORDER BY tree_left ASC LIMIT 1', treePid, bound);
		else
			dbr = orm.query('SELECT tree_left FROM {'+category.name+'} WHERE tree_pid IS NULL AND tree_left>=$1 ORDER BY tree_left ASC LIMIT 1', bound);
			
		if(dbr.length != 1)//влево некуда
			return v;

		concurentLeft = dbr[0].tree_left;
		concurentRight = targetLeft-1;
	}
	else
	{//вправо
		if(treePid)
			dbr = orm.query('SELECT tree_right FROM {'+category.name+'} WHERE tree_pid=$1 AND tree_right<=$2 ORDER BY tree_right DESC LIMIT 1', treePid, bound);
		else
			dbr = orm.query('SELECT tree_right FROM {'+category.name+'} WHERE tree_pid IS NULL AND tree_right<=$1 ORDER BY tree_right DESC LIMIT 1', bound);
			
		if(dbr.length != 1)//вправо некуда
			return v;

		concurentRight = dbr[0].tree_right;
		concurentLeft = targetRight+1;
	}
	

	let targetStep = moveLeft?(concurentLeft-targetLeft):(concurentRight-targetRight);
	let concurentStep = moveLeft?(targetRight-concurentRight):(targetLeft-concurentLeft);
	
	//теперь маркировать перемещаемый узел в новый tree_root
	//и двинуть перемещаемый
	let targetTemporaryTreeRoot = category.getNextInsertId();
	orm.exec('UPDATE {'+category.name+'} SET tree_root=$1, tree_left=tree_left+$2, tree_right=tree_right+$2 WHERE tree_left>=$3 AND tree_right<=$4 AND tree_root=$5', targetTemporaryTreeRoot, targetStep, targetLeft, targetRight, treeRoot);

	//двинуть конкурента
	orm.exec('UPDATE {'+category.name+'} SET tree_left=tree_left+$1, tree_right=tree_right+$1 WHERE tree_left>=$2 AND tree_right<=$3 AND tree_root=$4', concurentStep, concurentLeft, concurentRight, treeRoot);
	
	//вернуть перемещаемый в дерево
	orm.exec('UPDATE {'+category.name+'} SET tree_root=$1 WHERE tree_root=$2', treeRoot, targetTemporaryTreeRoot);
	
	v.tree_left = targetLeft + targetStep;
	v.tree_right = targetRight + targetStep;
	
	return v;
}









///////////////////////////////////////////////////////////
orm.addCategory({
	name:'NsTree',
	base:'BaseCategory',
	abstract:true,
	fields:
	{
		//id - pid классическое дерево, источник целостности, все остальные высчитываются по этому
		tree_pid:
		{
			type:'fkey',
			target:'NsTree',
		},

		//для разделения многих деревьяе в одной таблице маркер корня, одинаковый у всех элементов одного дерева
		tree_root:
		{
			type:'fkey',
			target:'NsTree',
		},

		//уровень вложенности, может пригодится?
		tree_level:'integer',

		//nestedSet левое значение
		tree_left:'integer',

		//nestedSet правое значение
		tree_right:'integer',
	},
	
	methods:
	{
		move:function(bound)
		{
			let category = orm[this.category];
			if(!category) throw Error('unknown category '+this.category);

			category.move(this, bound);
			return this;
		},
	},
	
	categoryMethods:
	{
		//////////////////////////////////////////////////////////
		insert: function insert(v)
		{
			let self = this;
			if(v.tree_pid)
			{
				asTrans(this, function() insertChildImpl(self, v));
			}
			else
			{
				asTrans(this, function() insertRootImpl(self, v));
			}
		},

		//////////////////////////////////////////////////////////
		update: function update(v)
		{
			let self = this;
			if(v.tree_pid)
			{
				asTrans(this, function() updateChildImpl(self, v));
			}
			else
			{
				asTrans(this, function() updateRootImpl(self, v));
			}
		},
		
		//////////////////////////////////////////////////////////
		save: function save(v)
		{
			if(v.id) return this.update(v);
			return this.insert(v);
		},
		
		//////////////////////////////////////////////////////////
		move: function move(v, bound)
		{
			let self = this;
			asTrans(this, function() moveImpl(self, v, bound));
		},
		

		//////////////////////////////////////////////////////////
		delete: function delete(v)
		{
			let self = this;
			asTrans(this, function() 
				orm.exec('DELETE FROM {'+self.name+'} WHERE tree_left>=$1 AND tree_right<=$2 AND tree_root=$3', 
					v.tree_left, 
					v.tree_right,
					v.tree_root));
		},
		
		
		
		///////////////////////////////////////////////////////////
		linearToHierarchy: function linearToHierarchy(array)
		{
			let elementsById = {};
			let roots = [];
			for each(let element in array)
			{
				element.childs = [];
				elementsById[element.id] = element;
			}

			for each(let element in array)
			{
				if(element.tree_pid)
				{
					element.parent = elementsById[element.tree_pid];
					if(element.parent)
					{
						element.parent.childs.push(element);
					}
				}
				else
				{
					roots.push(element);
				}
			}
			
			return roots;
		}
		
		
		
		
		
		
		
		
		
	}
});

