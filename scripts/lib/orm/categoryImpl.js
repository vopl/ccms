//////////////////////////////////////////////////////////
let impl = 
{
	conf:arguments[0],
	iface:arguments[1],
	ormImpl:arguments[2],
	
	bases:{},
	allBases:{},
	allBasesWithSelf:{},
	deriveds:{},
	allDeriveds:{},
	allDerivedsWithSelf:{},
	
	fieldImpls:{},
	methods:{},
	categoryMethods:{},
	constraints:{},
	indices:{},
	validator:null,
};

//////////////////////////////////////////////////////////
impl.iface.getImpl = function getImpl()
{
	return impl;
}
he(impl.iface, 'getImpl');

//////////////////////////////////////////////////////////
impl.getBases = function getBases()
{
	return impl.bases;
}

//////////////////////////////////////////////////////////
impl.getAllBases = function getAllBases()
{
	return impl.allBases;
}

//////////////////////////////////////////////////////////
impl.getDeriveds = function getDeriveds()
{
	return impl.deriveds;
}

//////////////////////////////////////////////////////////
impl.getAllDeriveds = function getAllDeriveds()
{
	return impl.allDeriveds;
}

//////////////////////////////////////////////////////////
impl.updateHierarchy = function updateHierarchy()
{
	//построить кэш иерархии
	this.bases = {};//прямые базовые
	this.allBases = {};//все базовые рекурсивно
	this.allBasesWithSelf = {};//все базовые рекурсивно вместе со своими
	this.deriveds = {};//прямые производные
	this.allDeriveds = {};//все производные рекурсивно
	this.allDerivedsWithSelf = {};//все производные рекурсивно вместе со своими

	let impl = this;
	
	/////////////////////////////////////////////////////////////////////////
	//this.bases
	//this.allBases = {};
	function bases_builder(level, categoryImpl)
	{
		if(1 == level) impl.bases[categoryImpl.conf.name] = categoryImpl;
		if(1 <= level) impl.allBases[categoryImpl.conf.name] = categoryImpl;
		impl.allBasesWithSelf[categoryImpl.conf.name] = categoryImpl;
		for each(let name in categoryImpl.conf.inherits)
		{
			bases_builder(level+1, impl.ormImpl.iface.categories[name].getImpl());
		}
		for each(let name in categoryImpl.conf.bases)
		{
			bases_builder(level+1, impl.ormImpl.iface.categories[name].getImpl());
		}
	}
	bases_builder(0, this);
	

	

	/////////////////////////////////////////////////////////////////////////
	//this.deriveds = {};
	//this.allDeriveds = {};
	function deriveds_builder(level, thisCategoryImpl)
	{
		if(1 == level) impl.deriveds[thisCategoryImpl.conf.name] = thisCategoryImpl;
		if(1 <= level) impl.allDeriveds[thisCategoryImpl.conf.name] = thisCategoryImpl;
		impl.allDerivedsWithSelf[thisCategoryImpl.conf.name] = thisCategoryImpl;
		//не те объемы чтобы жаловаться на полный кросс
		for(let name in impl.ormImpl.iface.categories)
		{
			let categoryImpl = impl.ormImpl.iface.categories[name].getImpl();
			for each(let baseName in categoryImpl.conf.inherits)
			{
				if(baseName == thisCategoryImpl.conf.name)
				{
					deriveds_builder(level+1, categoryImpl);
				}
			}
			for each(let baseName in categoryImpl.conf.bases)
			{
				if(baseName == thisCategoryImpl.conf.name)
				{
					deriveds_builder(level+1, categoryImpl);
				}
			}
		}
	}
	deriveds_builder(0, this)

	//унаследовать 
	/*
		поля, 
		методы, 
		ограничения первичных и внешних ключей
		индексы
		валидаторы
	*/
	this.fieldImpls = {};
	this.constraints = {};
	this.indices = {};
	this.methods = {};
	this.categoryMethods = {};
	this.validator = null;
	
	for each(let baseOrSelf in this.allBasesWithSelf)
	{
		for(let fname in baseOrSelf.conf.fields)
		{
			if(!(fname in this.fieldImpls))
			{
				let field = baseOrSelf.conf.fields[fname];
				this.fieldImpls[fname] = exec('fieldImpls/'+field.type+'.js', field);
				switch(field.type)
				{
				case 'fkey':
					this.constraints[fname] = field;
					break;
				case 'pkey':
					this.constraints[fname] = field;
					break;
				}
			}
		}

		for(let iname in baseOrSelf.conf.indices)
		{
			if(!(iname in this.indices))
			{
				let index = baseOrSelf.conf.indices[iname];
				this.indices[iname] = index;
			}
		}
		
		for(let mname in baseOrSelf.conf.methods)
		{
			if(!(mname in this.methods))
			{
				let method = baseOrSelf.conf.methods[mname];
				this.methods[mname] = method;
			}
		}
		for(let mname in baseOrSelf.conf.categoryMethods)
		{
			if(!(mname in this.categoryMethods))
			{
				let method = baseOrSelf.conf.categoryMethods[mname];
				this.categoryMethods[mname] = method;
			}
		}
		
		if(baseOrSelf.conf.validator)
		{
			if(!this.validator)
			{
				this.validator = baseOrSelf.conf.validator;
			}
		}
	}
}

impl.schema = impl.ormImpl.iface.schema;
impl.tableName = impl.conf.name;
impl.fullTableName = '"' + impl.ormImpl.iface.schema + '"."' + impl.conf.name + '"';
impl.fullPkSequenceName = '"' + impl.ormImpl.iface.schema + '"."common_id_seq"';


//////////////////////////////////////////////////////////
impl.needSync = function needSync(options)
{
	let db = this.ormImpl.iface.db;
	
	
	//таблица
	let dbr = db.query('SELECT * FROM information_schema.tables WHERE table_schema=$1 AND table_name=$2', this.schema, this.tableName);
	if(!dbr.length)
	{
		//таблицы нет
		return true;
	}

	//лишние поля
	if(options.dropField == 'allow')
	{
		dbr = db.query('SELECT * FROM information_schema.columns WHERE table_schema=$1 AND table_name=$2', this.schema, this.tableName);
		for each(let col in dbr)
		{
			if(!(col.column_name in this.fieldImpls))
			{
				//лишнее поле
				return true;
			}
		}
	}

	//недостающие поля или существующие
	for(let fname in this.fieldImpls)
	{
		let fieldImpl = this.fieldImpls[fname];
		//проверить если уже есть
		dbr = db.query('SELECT * FROM information_schema.columns WHERE table_schema=$1 AND table_name=$2 AND column_name=$3', this.schema, this.tableName, fname);
		if(dbr.length)
		{//колонка есть
			if(dbr[0].data_type != fieldImpl.dbTypeCheck)
			{
				//колонка вне формата
				return true;
			}
		}
		else
		{//колонки нет
			return true;
		}
	}	
	
	//ограничения
	return true;

	//индексы
	return true;

	//наследование
	return true;
	
	return false;
}

//////////////////////////////////////////////////////////
//для таблицы из базы бросить все ограничения
impl.dropConstraints1 = function dropConstraints1(options)
{
	let db = this.ormImpl.iface.db;
	for(;;)
	{
		//let dbr = db.query('SELECT * FROM information_schema.constraint_table_usage WHERE table_schema=$1 AND constraint_schema=$1 AND table_name=$2', this.schema, this.tableName);
		let dbr = db.query('SELECT * FROM information_schema.key_column_usage WHERE table_schema=$1 AND constraint_schema=$1 AND table_name=$2', this.schema, this.tableName);
		if(dbr.length)
		{
			if('allow' != options.dropConstraint)
			{
				throw "unable to drop constraint, options.dropConstraint != 'allow'";
			}
			db.exec('ALTER TABLE "'+dbr[0].table_schema+'"."'+dbr[0].table_name+'" DROP CONSTRAINT "'+dbr[0].constraint_name+'" CASCADE');
			continue;
		}
		break;
	}
}

//////////////////////////////////////////////////////////
//для таблицы из базы бросить все ограничения
impl.dropConstraints2 = function dropConstraints2(options)
{
	let db = this.ormImpl.iface.db;
	//бросить not null, default
	let dbr = db.query('SELECT * FROM information_schema.columns WHERE table_schema=$1 AND table_name=$2', this.schema, this.tableName);
	for each(let col in dbr)
	{
		if(!col.is_nullable != "YES")
		{
			db.exec('ALTER TABLE '+this.fullTableName+' ALTER COLUMN "'+col.column_name+'" DROP NOT NULL');
		}
		if(col.column_default != null)
		{
			db.exec('ALTER TABLE '+this.fullTableName+' ALTER COLUMN "'+col.column_name+'" DROP DEFAULT');
		}
	}
}

//////////////////////////////////////////////////////////
//для таблицы из базы бросить все индексы
impl.dropIndices = function dropIndices(options)
{
	let db = this.ormImpl.iface.db;
	for(;;)
	{
		let dbr = db.query('SELECT * FROM pg_catalog.pg_indexes WHERE schemaname=$1 AND tablename=$2', this.schema, this.tableName);
		if(dbr.length)
		{
			if('allow' != options.dropIndex)
			{
				throw "unable to drop constraint, options.dropIndex != 'allow'";
			}
			db.exec('DROP INDEX "'+dbr[0].schemaname+'"."'+dbr[0].indexname+'" CASCADE');
			continue;
		}
		break;
	}
}


//////////////////////////////////////////////////////////
//для таблицы из базы бросить наследование вверх и вниз
impl.dropInheritance = function dropInheritance(options)
{
	let db = this.ormImpl.iface.db;
	for(;;)
	{
		let dbr = db.query('SELECT tb.relname as bname, tb.schemaname as bschema, td.relname as dname, td.schemaname as dschema FROM pg_catalog.pg_inherits as inh'+
		' LEFT JOIN pg_catalog.pg_stat_all_tables as td ON(inh.inhrelid=td.relid)'+
		' LEFT JOIN pg_catalog.pg_stat_all_tables as tb ON(inh.inhparent=tb.relid)'+
		' WHERE td.schemaname=$1 AND td.relname=$2 OR tb.schemaname=$1 AND tb.relname=$2', this.schema, this.tableName);
		if(dbr.length)
		{
			if('allow' != options.dropInheritance)
			{
				throw "unable to drop constraint, options.dropInheritance != 'allow'";
			}
			
			db.exec('ALTER TABLE "'+dbr[0].dschema+'"."'+dbr[0].dname+'" NO INHERIT "'+dbr[0].bschema+'"."'+dbr[0].bname+'"');
			continue;
		}
		break;
	}
}

//////////////////////////////////////////////////////////
//создать или обновить таблицы
impl.createTables = function createTables(options)
{
	if(this.conf.abstract) return;
	
	let db = this.ormImpl.iface.db;
	//если таблицы нет то создать пустую
	let dbr = db.query('SELECT * FROM information_schema.tables WHERE table_schema=$1 AND table_name=$2', this.schema, this.tableName);
	if(!dbr.length)
	{
		if('allow' != options.createTable)
		{
			throw "unable to create table, options.createTable != 'allow'";
		}
		db.exec('CREATE TABLE '+this.fullTableName+'()');
	}

	//срезать лишние поля
	dbr = db.query('SELECT * FROM information_schema.columns WHERE table_schema=$1 AND table_name=$2', this.schema, this.tableName);
	for each(let col in dbr)
	{
		if(!(col.column_name in this.fieldImpls))
		{
			if(options.dropField != 'allow')
			{
				warn('unable to drop column '+this.fullTableName+'."'+col.column_name+'", options.dropField != \'allow\'');
			}
			else
			{
				db.exec('ALTER TABLE '+this.fullTableName+' DROP COLUMN "'+col.column_name+'"');
			}
		}
	}

	//добавить недостающие поля или обновить существующие
	for(let fname in this.fieldImpls)
	{
		let fieldImpl = this.fieldImpls[fname];
		//проверить если уже есть
		dbr = db.query('SELECT * FROM information_schema.columns WHERE table_schema=$1 AND table_name=$2 AND column_name=$3', this.schema, this.tableName, fname);
		if(dbr.length)
		{//колонка есть, корректировать
			if(dbr[0].data_type != fieldImpl.dbTypeCheck)
			{
				if('allow' != options.alterField)
				{
					throw "unable to alter field to change type, options.alterField != 'allow'";
				}
				db.exec('ALTER TABLE '+this.fullTableName+' ALTER COLUMN "'+fname+'" TYPE '+fieldImpl.dbTypeSet);
			}
		}
		else
		{//колонки нет, создать
			if('allow' != options.createField)
			{
				throw "unable to add column, options.createField != 'allow'";
			}
			db.exec('ALTER TABLE '+this.fullTableName+' ADD COLUMN "'+fname+'" '+fieldImpl.dbTypeSet);
		}
	}
}

//////////////////////////////////////////////////////////
//создать иерархию наследования
impl.createInheritance = function createInheritance(options)
{
	if(this.conf.abstract) return;
	let db = this.ormImpl.iface.db;
	
	for each(let baseName in this.conf.inherits)
	{
		if('allow' != options.createInheritance)
		{
			throw "unable to fill default values, options.createInheritance != 'allow'";
		}
		if(this.ormImpl.iface[baseName].getImpl().conf.abstract) continue;
		db.exec('ALTER TABLE '+this.fullTableName+' INHERIT "'+this.schema+'"."'+baseName+'"');
	}
}

//////////////////////////////////////////////////////////
//привести данные в соответствие с ограничениями
impl.correctData = function correctData(options)
{
	if(this.conf.abstract) return;
	let db = this.ormImpl.iface.db;
	
	for (let fname in this.fieldImpls)
	{
		let fieldImpl = this.fieldImpls[fname];
		
		//not null <- default
		if(fieldImpl.notNull)
		{
			if('allow' != options.fillDefault)
			{
				throw "unable to fill default values, options.fillDefault != 'allow'";
			}
			db.exec('UPDATE '+this.fullTableName+' SET "'+fname+'"=$1 WHERE "'+fname+'" IS NULL', fieldImpl.default);
		}
		
		//pkey, fkey
	}
}

//////////////////////////////////////////////////////////
//создать ограничения
impl.createConstraints1 = function createConstraints1(options)
{
	if(this.conf.abstract) return;
	let db = this.ormImpl.iface.db;
	
	for (let fname in this.fieldImpls)
	{
		let fieldImpl = this.fieldImpls[fname];
		
		//pkey
		if('pkey' == fieldImpl.type)
		{
			if('allow' != options.createConstraint)
			{
				throw "unable to create primary key constraint, options.createConstraint != 'allow'";
			}
			db.exec('ALTER TABLE '+this.fullTableName+' ADD PRIMARY KEY ("'+fname+'")');
		}
	}
}

//////////////////////////////////////////////////////////
//создать ограничения
impl.createConstraints2 = function createConstraints2(options)
{
	if(this.conf.abstract) return;
	let db = this.ormImpl.iface.db;
	
	for (let fname in this.fieldImpls)
	{
		let fieldImpl = this.fieldImpls[fname];

		//fkey
		if('fkey' == fieldImpl.type)
		{
			let targetCategory = this.ormImpl.iface.categories[fieldImpl.target];
			if(!targetCategory)
			{
				throw "target category "+fieldImpl.target+" via foreign key "+this.conf.name+'.'+fname+' absent';
			}
			
			let targetCategoryImpl = targetCategory.getImpl();
			
			if(!('id' in targetCategoryImpl.fieldImpls))
			{
				throw "target category "+fieldImpl.target+" via foreign key "+this.conf.name+'.'+fname+' has no primary key "id"';
			}
			if('pkey' != targetCategoryImpl.fieldImpls.id.type)
			{
				throw "target category "+fieldImpl.target+" via foreign key "+this.conf.name+'.'+fname+' has wrong type for "id" (must be "pkey")';
			}
			
			if('allow' != options.createConstraint)
			{
				throw "unable to create foreign key constraint, options.createConstraint != 'allow'";
			}
/*
http://www.postgresql.org/docs/8.4/static/ddl-inherit.html		
5.8.1. Caveats
...
Specifying that another table's column REFERENCES cities(name) would allow the other 
table to contain city names, but not capital names. There is no good workaround for this case.
*/
			db.exec(
				'ALTER TABLE '+this.fullTableName+
				' ADD FOREIGN KEY ("'+fname+'")'+
				' REFERENCES '+targetCategoryImpl.fullTableName+'("id")'+
				' ON UPDATE '+fieldImpl.onUpdate+
				' ON DELETE '+fieldImpl.onDelete+
				' DEFERRABLE INITIALLY DEFERRED');
//*/		
		}
		//notNull
		if(fieldImpl.notNull)
		{
			db.exec('ALTER TABLE '+this.fullTableName+' ALTER COLUMN "'+fname+'" SET NOT NULL');
		}
		//default
		if('pkey' == fieldImpl.type)
		{
			db.exec('ALTER TABLE '+this.fullTableName+' ALTER COLUMN "'+fname+'" SET DEFAULT nextval(\''+this.fullPkSequenceName+'\'::regclass)');
		}
		else
		{
			if(fieldImpl.default != null)
			{
				db.exec('ALTER TABLE '+this.fullTableName+' ALTER COLUMN "'+fname+'" SET DEFAULT \''+db.q(fieldImpl.default)+"'");
			}
		}
	}
}



//////////////////////////////////////////////////////////
//создать индексы
impl.createIndices = function createIndices(options)
{
	let db = this.ormImpl.iface.db;
	
	for (let iname in this.indices)
	{
		let index = this.indices[iname];

		let columns = [];
		for each(let c in index.targets)
		{
			columns.push('"'+c+'"');
		}
		columns = columns.join(',');
		
		if('allow' != options.createIndex)
		{
			throw "unable to create index, options.createIndex != 'allow'";
		}
		db.exec('CREATE '+(index.unique?"UNIQUE ":"")+'INDEX "index_'+this.tableName+'_'+iname+'" ON '+this.fullTableName+'('+columns+')')
	}
}


//////////////////////////////////////////////////////////
//выбрать oid собственной таблицы
impl.selectTableoid = function selectTableoid()
{
	let db = this.ormImpl.iface.db;
	let dbr = db.query('SELECT c.oid FROM pg_class c '+
		'LEFT JOIN pg_namespace n on(c.relnamespace=n.oid) '+
		'WHERE n.nspname=$1 AND c.relname=$2', this.schema, this.tableName);
	return dbr[0].oid;
}


//////////////////////////////////////////////////////////
//создать индексы
impl.setSyncedFlag = function setSyncedFlag(flag)
{
	this.synced = flag;
	this.iface.__proto__ = this.categoryMethods;
}











//////////////////////////////////////////////////////////
impl.insert = function insert(v)
{
	let db = this.ormImpl.iface.db;
	let fnamesSql = [];
	let fvaluesSql = [];
	let fvalues = [];
	for(let fname in this.fieldImpls)
	{
		if('id' == fname) continue;
		if(fname in v)
		{
			fvaluesSql.push('$'+(fvaluesSql.length+1));
			fvalues.push(v[fname]);
			fnamesSql.push('"'+fname+'"');
		}
		else
		{
		}
	}
	if(!fvaluesSql.length)
	{
		fvaluesSql.push('DEFAULT');
		fnamesSql.push('"id"');
	}
	
	let sql = 'INSERT INTO '+this.fullTableName+' ('+fnamesSql.join(',')+') VALUES ('+fvaluesSql.join(',')+')'
	let stmt = this.ormImpl.getPraparedStatement(sql);
	stmt.exec(fvalues);
	
	v.id = this.getLastInsertId();
	return v;
}


//////////////////////////////////////////////////////////
impl.update = function update(v)
{
	if(!v.id) return false;
	let db = this.ormImpl.iface.db;
	let fvaluesSql = [];
	let fvalues = [];
	for(let fname in this.fieldImpls)
	{
		if(fname in v)
		{
			if('id' != fname)
			{
				fvaluesSql.push('"'+fname+'"=$'+(fvaluesSql.length+1));
				fvalues.push(v[fname]);
			}
		}
		else
		{
		}
	}
	if(!fvaluesSql.length)
	{
		fvaluesSql.push('id=id');
	}

	let sql = 'UPDATE '+this.fullTableName+' SET '+fvaluesSql.join(',')+' WHERE id=$'+(fvaluesSql.length+1);
	fvalues.push(v.id);
	
	let stmt = this.ormImpl.getPraparedStatement(sql);
	stmt.exec(fvalues);
	return v;
}

//////////////////////////////////////////////////////////
impl.save = function save(v)
{
	if(v.id) return this.update.call(this, v);
	return this.insert.call(this, v);
}

//////////////////////////////////////////////////////////
impl.delete = function delete(v)
{
	if(!v.id) return false;
	let stmt = this.ormImpl.getPraparedStatement('DELETE FROM '+this.fullTableName+' WHERE id=$1');
	stmt.exec(v.id);
	delete v.id;
	return v;
}

//////////////////////////////////////////////////////////
impl.select = function select(sql2part)
{
	let sql = "SELECT tableoid, * FROM "+this.fullTableName+" "+sql2part;
	let values = Array.prototype.slice.call(arguments, 1);
	
	let stmt = this.ormImpl.getPraparedStatement(sql);
	let arr = stmt.query(values);
	
	return this.adoptArray(arr);
}

//////////////////////////////////////////////////////////
impl.load = function load(v)
{
	let sql = "SELECT tableoid, * FROM "+this.fullTableName+" WHERE id=$1";
	
	let stmt = this.ormImpl.getPraparedStatement(sql);
	let rec = stmt.query(v.id)[0];
	
	if(!rec) return false;
	for(let pname in rec)
	{
		v[pname] = rec[pname];
	}
	
	return this.adopt(v);
}

//////////////////////////////////////////////////////////
impl.adoptArray = function adoptArray(arr)
{
	//перебрать массив
	//если категория отличается из производных - довыбрать поля
	for each(let rec in arr)
	{
		let tableoid = rec.tableoid;
		if(tableoid)
		{
			let categoryImpl = this.ormImpl.tableoid2categoryImpl[tableoid];
			
			if(categoryImpl)
			{
				if(!this.allDerivedsWithSelf[categoryImpl.conf.name])
				{
					throw Error("wtf? Requested category is not derived from this category");
				}
				categoryImpl.adopt(rec, this);
			}
		}
	}
	return arr;
}

//////////////////////////////////////////////////////////
impl.adopt = function adopt(rec, fromCategoryImpl)
{
	delete rec.tableoid;
	rec.category = this.conf.name;
	//насунуть методы
	rec.__proto__ = this.methods;
	
	return rec;
}

//////////////////////////////////////////////////////////
impl.make = function make(alienRec)
{
	let rec = {};
	rec.category = this.conf.name;
	for(let fname in this.fieldImpls)
	{
		rec[fname] = alienRec[fname];
	}
	//насунуть методы
	rec.__proto__ = this.methods;
	
	return rec;
}


//////////////////////////////////////////////////////////
impl.getLastInsertId = function getLastInsertId()
{
	let sql = 'SELECT currval($1)';
	let stmt = this.ormImpl.getPraparedStatement(sql);
	let dbr = stmt.query(this.fullPkSequenceName);
	return dbr[0]['currval'];
};


//////////////////////////////////////////////////////////
impl.getNextInsertId = function getNextInsertId()
{
	let sql = 'SELECT nextval($1)';
	let stmt = this.ormImpl.getPraparedStatement(sql);
	let dbr = stmt.query(this.fullPkSequenceName);
	return dbr[0]['nextval'];
};

//////////////////////////////////////////////////////////
impl.ormImpl.iface.categories[impl.conf.name] = impl.iface;
return impl;